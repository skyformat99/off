/* off.c
 *
 * C implementation for the "Ois File Format".
 *
 * Copyright (C) 2017 bindh3x <os@bindh3x.io>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 3, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <string.h>
#include <ctype.h>
#include "off.h"

int off_cchr(char *s, int c) {
  int i = 0;

  while ( *s ) {
    if ( *s == c ) i++;
    s++;
  }
  return i;
}

int off_parse_line(char *line, offline_t *l) {
  offstate_t st = OFF_STATE_IN_KEY;
  int i = 0;

  /* Check if the line starts with comment or new-line */
  if ( *line == OFF_COMMENT || *line == 0x0a ) {
    return OFF_SYNTAX_PASS;
  }

  /* Remove new line character from the line */
  if ( line[strlen(line)-1] == 0x0a ) {
    line[strlen(line)-1] = 0x00;
  }

  for ( i = 0; line[i]; i++ ) {
    /* Invalid syntax */
    if ( st == OFF_STATE_IN_KEY && 
      line[i] == 0x0a &&
      i > 0 ) {
      return OFF_SYNTAX_INVALID;      
    }

    if ( st == OFF_STATE_IN_KEY ) {
      if ( line[i] == 0x20 ) {
	st = OFF_STATE_IN_VALUE;
	l->k.end = i;
	l->v.start = i+1;
	continue;
      }
    }
    else if ( st ==  OFF_STATE_IN_VALUE ) {
      /* nothing */
    }
  }
  l->v.end = i;
  l->v.type = off_guess_type(line, &l->v);

  return OFF_SYNTAX_OK;
}

void off_new_line(offline_t *l) {
  l->k.start = 0;
  l->k.end   = 0;
  l->k.type  = 0;
  l->v.start = 0;
  l->v.end   = 0;
  l->v.type  = 0;
}

offtype_t off_guess_type(char *line, offtok_t *tok) {

  /* Array */
  if ( line[strlen(line)-1] == OFF_ARRAY_CLOSE && 
      off_cchr(line, OFF_ARRAY_OPEN) == 1 && 
      off_cchr(line, OFF_ARRAY_CLOSE) == 1 ) {
    return OFF_TYPE_ARRAY;   
  }
  /* Boolean false */
  else if ( off_tokcmp(line, tok, "false") == 0 ) {
    return OFF_TYPE_BOOLEAN_FALSE;
  }
  /* Boolean true */
  else if ( off_tokcmp(line, tok, "true") == 0 ) {
    return OFF_TYPE_BOOLEAN_TRUE;
  }
  /* Integer */
  else if ( off_isint(line, tok) ) {
    return OFF_TYPE_INTEGER;
  }
  /* by default the value is a string */
  return OFF_TYPE_STRING;
}

offtype_array_t off_parse_array(char *line) {
  int i = 0;
  offtype_array_t ar;
  offstate_t st = 0;
  ar.el = 0;
  ar.size = 0;

  for ( i = 0; line[i]; i++ ) {
    if ( line[i] == OFF_ARRAY_OPEN ) {
      st = OFF_STATE_IN_ARRAY;
      ar.start = i;
      ar.el++;
      ar.size++;
      continue;        
    }
    if ( st == OFF_STATE_IN_ARRAY ) {
      if ( line[i]  == OFF_ARRAY_ELEMENT_SEPARATOR ) ar.el++;
      else if ( line[i] == OFF_ARRAY_CLOSE ) {
	ar.end = i;
	break;
      }
      ar.size++;
    }
  }
  return ar;
}

offtok_t off_array_get_element(char *line, int idx, offtype_array_t *array) {
  offtok_t tok;
  tok.end = 0;
  tok.start = array->start;
  char *lp = &line[array->start];
  int pos = array->start; /* current position */
  int index = 0;

  while ( *lp ) {
    if ( *lp == OFF_ARRAY_ELEMENT_SEPARATOR ) {
      tok.end = pos;
      if ( idx == array->el-1 ) {
	/* if we are in the last element,
	 * set tok->end to array->end */
	tok.end = array->end;
      }
      if ( index == idx ) break;
      tok.start = pos+1; /* skip OFF_ARRAY_ELEMENT_SEPARATOR */
      index++;
    }
    lp++;
    pos++;
  }
  if ( idx == 0 ) tok.start++; /* skip OFF_ARRAY_OPEN, "{element" --> "element" */
  line[array->start] = 0x3c; /* guess_type will not detect this as an array */
  tok.type = off_guess_type(line, &tok);
  line[array->start] = OFF_ARRAY_OPEN; /* now it will be detected as an array */

  return tok; 
}

size_t off_get_toklen(char *line, offtok_t *tok) {
  return (tok->end - tok->start);
}

int off_tokcmp(char *line, offtok_t *tok, char *str) {
  size_t tok_len = off_get_toklen(line, tok);

  if ( tok_len == strlen(str) &&
   strncmp(line + tok->start, str, tok_len) == 0 ) return 0;
  return -1;
}

int off_isint(char *line, offtok_t *tok) {
  char *lp = &line[tok->start];
  int i = 0;
  
  while ( *lp ) {
    if ( isdigit(*lp )) i++; 
    lp++;
  }
  if ( i == off_get_toklen(line, tok) ) return 1;
  return 0;
}
