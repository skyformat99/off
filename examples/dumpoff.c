/* dumpoff.c  
 * simple example for parsing off file
 *
 * Copyright (c) 2017 bindh3x <os@bindh3x.io>
 * 21-04-2017
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../off.h"

#define MAX_STR_LEN 256

void show_type(char*, offtok_t*);

void parse_array(char *line) {
  offtype_array_t ar;
  offtok_t tok;
  int i = 0;

  ar = off_parse_array(line);
  printf("elements in the array: %d\n", ar.el);

  for ( i = 0; i < ar.el; i++ ) {
    tok = off_array_get_element(line, i, &ar);
    printf("Index: %d - start: %d, end: %d\n", i, tok.start, tok.end);
    printf("Element: %.*s\n", tok.end - tok.start, line + tok.start);
    show_type(line, &tok);
  }
  putchar(0x0a);
}

void show_type(char *line, offtok_t *tok) {
  switch(tok->type) {
      case OFF_TYPE_ARRAY:
	printf("type: ARRAY\n\n");
	break;
      case OFF_TYPE_BOOLEAN_FALSE:     
	printf("type: BOOLEAN FALSE\n\n");
	break;
      case OFF_TYPE_BOOLEAN_TRUE:
	printf("type: BOOLEAN TRUE\n\n");
	break;
      case OFF_TYPE_INTEGER:
	printf("type: INTEGER\n\n");
	break;
      case OFF_TYPE_STRING:
	printf("type: STRING\n\n");
	break;
    }
}

int main(int argc, char **argv) {
  offline_t l;
  FILE *fp = NULL;
  char line[MAX_STR_LEN];
  int st = 0;
  int lines = 0;

  if ( argc < 2 ) {
    fprintf(stderr, "usage: %s [file] ...\n", argv[0]);
    exit(0);
  }

  if (( fp = fopen(argv[1], "rt")) == NULL ) {
    fprintf(stderr, "cannot open: %s\n", argv[1]);
    exit(0);
  }

  while (( fgets(line, 256, fp) ) != NULL ) { 
    off_new_line(&l);
    st = off_parse_line(line, &l);
    lines++;

    if ( st == OFF_SYNTAX_INVALID ) {
      fprintf(stderr, "invalid syntax, line %d\n", lines);
      break;
    }
    else if ( st == OFF_SYNTAX_PASS ) continue;

    printf("key start: %d, end: %d\n", l.k.start, l.k.end);
    printf("value start: %d, end: %d\n", l.v.start, l.v.end);
    printf("key  : %.*s\n", l.k.end - l.k.start, line + l.k.start);
    printf("value: %.*s\n", l.v.end - l.v.start, line + l.v.start);
    show_type(line, &l.v);
    if ( l.v.type == OFF_TYPE_ARRAY ) {
      parse_array(line);
    }
  }

  fclose(fp);
  return 0;
}
