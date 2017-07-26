# Off

Off is a lightweight format for small configuration files.
It aims to be minimal and efficient.

# Specifications

The syntax is easy to learn, and there is three important rules I should mention:

1. The value separator is space -> 0x20 (don't mistake with = ).
2. Syntax is case sensitive.
3. Every key & value should be in a new line.

Realistic example:

```lisp
;; HTTP Server configuration file
;; Author: bindh3x

;; server
host 0.0.0.0
port 8080
banner "HTTPServer 1.0"

;; privileges
run_as_root true
user root
group wheel

;; files
document_root {/var/www/html|/home/user/httpd/|/root/httpd}
index_file {index.html|index.htm|index.php|index.phtml}

;; admin contact information
admin_email admin@example.com
admin_phone +12345678
```

Off supports four primitive types: string, array, number and boolean.
by default any value will be treated as a string

## Types

1. String

String is bunch of characters grouping together.

```lisp
;; Good
my_awesome_url http://kernel.org/
email me@example.com
pgp 0xdeadbeef

;; Bad
my_awesome_url "Hello, World"
email "foo@bar"
``` 

2. Array

Array starts with { and ends with }, elements are separated by |.

```lisp
my_array {1|2|3|4|5|6|7|Hello, World|http://bindh3x.io}
```

3. Boolean

The boolean type has only two possible values: true and false. 
Use this type for simple flags that track true/false conditions. 

```lisp
;; Good
allow_user true
allow_root false

;; Bad
allow_user True
allow_root FALSE
```

4. Number

Number is integer and can be used for various operations.

```lisp
user_id 123 
age 100
```

## Comments

Comments should starts with ;.

```lisp
name john ;; This is my awesome name

; My last name
last_name doe
```

# Implementations

## C: liboff

Very minimal implementation written by bindh3x,
the API is very simple and you can find the documentation inside the off.h file

### Features

* No dynamic memory allocation.
* Very low footprint
* No dependencies!

See ./examples/ and ./tests/ for code examples.

Example:

```c
#include <stdio.h>
#include <stdlib.h>
#include "off.h"

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
```

### off-utils

The off-utils.c and off-utils.h files, are not officially part of liboff.
Mainly used to save time when developing with liboff.

# Hacking

For start you can read the HACKING.txt file.
If you want to add new feature, or fix something please open a pull request,
or send a patch to os@bindh3x.io with a proper description.

# Copying

[![license](https://img.shields.io/github/license/bindh3x/off.svg?style=flat-square)]()


