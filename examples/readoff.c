/* readoff.c  
 * simple example for parsing off file
 * ./readoff ../tests/array.off
 *
 * Copyright (c) 2017 bindh3x <os@bindh3x.io>
 * 23-04-2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../off.h"
#include "../off-utils.h"

#define MAX_STR_LEN 256

void parse_array(char *line) {
  offtype_array_t ar;
  offtok_t tok;
  char *ptr = NULL;

  ar = off_parse_array(line);
  /* Get index 2 from the array */
  tok = off_array_get_element(line, 2, &ar);
  /* compare the element value with the URL */
  if ( off_tokcmp(line, &tok, "https://jdoe.foo") == 0 ) {
    /* if there is a match extract the full string to a new heap variable */
    ptr = offutil_extract_string(line, &tok, ptr);
    printf("Token: %s\n", ptr);
    /* free always when using offutil_extract_string */
    free(ptr);  
  }
}

int main(int argc, char **argv) {
  FILE *fp;
  char buffer[MAX_STR_LEN];
  offstate_t st;
  offline_t l;

  if ( argc < 2 ) {
    fprintf(stderr, "usage: %s [file] ...\n", argv[0]);
    exit(0);
  }

  if (( fp = fopen(argv[1], "rt") ) == NULL ) {
    fprintf(stderr, "cannot open: %s\n", argv[1]);
    exit(0);
  }

  while (( fgets(buffer, sizeof(buffer), fp) ) ) {
    off_new_line(&l);
    st = off_parse_line(buffer, &l);
  
    if ( st == OFF_SYNTAX_OK )  {
      switch(l.v.type) {
	case OFF_TYPE_ARRAY:
	  parse_array(buffer);
	  break;
	case OFF_TYPE_STRING:
	case OFF_TYPE_BOOLEAN_TRUE:
	case OFF_TYPE_BOOLEAN_FALSE:
	case OFF_TYPE_INTEGER:
	  break;
      }  
    } 
  }

  fclose(fp);
  return 0;
}
