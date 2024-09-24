#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_to_hex(const char *str) {
  unsigned int len = strlen(str);
  char *hexstr = calloc(1, sizeof(char));

  for (unsigned int i = 0; i < len + 1; i++) {
    char *newstr = calloc(4, sizeof(char));
    hexstr =
        realloc(hexstr, (strlen(hexstr) + strlen(newstr) + 1) * sizeof(char));
    sprintf(newstr, "%x", str[len - i]);
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

char **str_to_hex_chunks(const char *str, int *n_chunks) {
  unsigned int len = strlen(str);
  unsigned int chunks = (len / 4) + 1;
  int *nptr = n_chunks;
  *nptr = chunks;

  char **str_list = calloc(chunks * 4, sizeof(char));

  for (unsigned int i = 0; i < (len / 4) + 1; i++) {
    char *chunk_str = mkstr(str + (i * 4));
    chunk_str = realloc(chunk_str, 4);
    chunk_str[4] = 0;
    char *hexstr = str_to_hex(chunk_str);

    str_list[i] = hexstr;
  }

  return str_list;
}
