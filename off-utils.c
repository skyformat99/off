/* off-utils.c
 *
 * off-utils is bunch of helper functions to work with liboff
 * the functions are not officially part of off, 
 * but will help you to do a simple tasks like 
 * extracting strings from tokens etc...
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
 * GNU General Public License for more details.-
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "off-utils.h"
#include "off.h"

void *offutil_malloc(size_t size) {
  void *p = NULL;

  if (( p = malloc(size)) == NULL ) {
    return NULL;
  }
  return p;
}

/* @brief: clone one by one string to malloc buffer
 * @d : dest buffer
 * @s : src buffer
 * @n : size to copy
 */
void chrclone(char *d, char *s, size_t n) {
  int i = 0;
  while ( *s ) {
    memcpy(&*d, &*s, 1);
    if ( i == n ) break;
    s++;
    d++;
    i++;
  }
  *d = '\0';
}

/* @brief extract string from offtok_t into allocated buffer
 * @paran line: line to parse in off syntax.
 * @param tok: offtok_t, token to the string.
 * @param ptr: pointer to a new NULL pointer.
 * @return the address of `ptr` with the new string.
 * on fail it will return NULL
 * XXX: call free() when done.
 */
char *offutil_extract_string(char *line, offtok_t *tok, char *ptr) {
  size_t tok_len = 0;

  tok_len = off_get_toklen(line, tok);

  if (( ptr = offutil_malloc(tok_len+1) ) != NULL ) {
    chrclone(ptr, line + tok->start, tok_len);
  }

  return ptr;
}

