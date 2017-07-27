/* off.h
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
#ifndef _NG_OFF
#define _NG_OFF

#define OFF_COMMENT                  0x3b
#define OFF_ARRAY_OPEN               0x7b
#define OFF_ARRAY_CLOSE              0x7d
#define OFF_ARRAY_ELEMENT_SEPARATOR  0x7c
#define OFF_SYNTAX_OK 0x03
#define OFF_SYNTAX_PASS 0x02
#define OFF_SYNTAX_INVALID 0x01

typedef enum {
  OFF_STATE_IN_KEY=1,
  OFF_STATE_IN_VALUE,
  OFF_STATE_IN_ARRAY,
} offstate_t;

typedef enum {
  OFF_TYPE_ARRAY=1,
  OFF_TYPE_STRING,
  OFF_TYPE_BOOLEAN_TRUE,
  OFF_TYPE_BOOLEAN_FALSE,
  OFF_TYPE_INTEGER,
} offtype_t;

typedef struct {
  int start; /* start index */
  int end; /* end index */
  offtype_t type; /* type of the token see above OT_* */
} offtok_t;

typedef struct {
  offtok_t k; /* key */
  offtok_t v; /* value */
} offline_t;

typedef struct {
  int el; /* number of elements in the array */
  int start; /* start position */
  int end; /* end position */
  size_t size; /* length of the array */
} offtype_array_t;

/* @brief parse one line in off syntax, the line most be valid.
 * the given `token` is going to store the information for the `line`
 *
 * @param line: line to parse
 * @param l   : offline_t, line tokens contain offtok_t for key,value.
 * @return offstate_t;
 */
int off_parse_line(char *line, offline_t *l);

/* @brief reset the offline_t for reading a new line.
 * @param l: offline_t struct
 */
void off_new_line(offline_t *l);

/* @brief guess the type of a line
 * @param line: line to guess
 * @param s   : size of line
 * @return offtype_t
 */
offtype_t off_guess_type(char *line, offtok_t *tok);

/* @brief count how many time character appears in a string.
 * @param string
 * @param character (i.e: 0x20 )
 */
int off_cchr(char *s, int c);

/* @brief parse array and return information for this array
 * @param line: line with array type to parse
 * @return    : offtype_array_t sturct, for the array
 */
offtype_array_t off_parse_array(char *line);

/* @brief return token for element in array
 * @param line: line with array type to parse
 * @param idx : index for the element 
 * @param array : offtype_array_t array from off_parse_array
 * @return offtok_t for the element
 */
offtok_t off_array_get_element(char *line, int idx, offtype_array_t *array);

/* @brief get length of a token
 * @param line: line to parse
 * @param tok: offtok_t
 * @return the length of the token string.
 */
size_t off_get_toklen(char *line, offtok_t *tok);

/* @brief compare token to a string.
 * @param line: line to parse
 * @param token: offtok_t
 * @param str  : string to compare.
 * @return 0 if equal, -1 if not.
 */
int off_tokcmp(char *line, offtok_t *tok, char *str);

/* @brief check if given string is integer
 * @param line: line to parse
 * @param tok: off token to check
 * @return 1 on success.
 */
int off_isint(char *line, offtok_t *tok);

#endif /* _NG_OFF */
