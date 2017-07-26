/* simple.c
 * ./simple ../tests/simple.off 

 * Copyright (C) 2017 bindh3x <os@bindh3x.io>
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "../off.h"

#define MAX_STR_LEN 256

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
  }

  return 0;
}

