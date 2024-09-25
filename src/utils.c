#include "include/utils.h"
#include "include/list.h"
#include "include/macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void assert_not_null(void *x) {
  if (!x)
    return;

  printf("\e[31m[Assertion Failure]: %p\e[0m\n", x);
  exit(1);
}

char *str_to_hex(const char *str) {
  unsigned int len = strlen(str);
  char *hexstr = calloc(1, sizeof(char));

  for (unsigned int i = 0; i < len + 1; i++) {
    char *newstr = calloc(4, sizeof(char));
    sprintf(newstr, "%x", str[len - i]);
    hexstr =
        realloc(hexstr, (strlen(hexstr) + strlen(newstr) + 1) * sizeof(char));
    strcat(hexstr, newstr);
    free(newstr);
  }

  return hexstr;
}

char *mkstr(const char *str) {
  char *out = (char *)calloc(strlen(str) + 1, sizeof(char));
  strcpy(out, str);

  return out;
}

list_T *str_to_hex_chunks(const char *str) {
  list_T *list = init_list(sizeof(char *));

  unsigned int i = 0;
  char *tmp = calloc(1, sizeof(char));

  while (str[i] != '\0') {
    tmp = realloc(tmp, (strlen(tmp) + 2) * sizeof(char));
    strcat(tmp, (char[]){str[i], 0});

    if ((i > 0 && (i - 1) % 4 == 0) || str[i] == '\n' || str[i] == '\t') {
      char *hex = str_to_hex(tmp);
      free(tmp);
      list_push(list, hex);
      tmp = calloc(1, sizeof(char));
    }

    i += 1;
  }

  return list;
}

char *format_str(char *str) {
  char *nstr = calloc(1, sizeof(char));
  unsigned int len = strlen(str);

  unsigned int i = 0;
  while (str[i] != '\0' && i < len) {
    nstr = realloc(nstr, (strlen(nstr) + 2) * sizeof(char));

    if (str[i] == '\\') {
      char esc = str_to_esc_seq((char[]){str[i], str[MIN(i + 1, len)], 0});
      strcat(nstr, (char[]){esc, 0});
      i += 2;
    } else {
      strcat(nstr, (char[]){str[i], 0});
      i += 1;
    }
  }

  return nstr;
}

char str_to_esc_seq(const char *str) {
  if (strlen(str) <= 1)
    return 0;

  if (str[0] != '\\')
    return 0;

  char in_c = str[1];

  switch (in_c) {
  case 'n':
    return '\n';
  case 't':
    return '\t';
  case '\\':
    return in_c;
  default:
    return in_c;
  }

  return in_c;
}
