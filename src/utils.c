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
