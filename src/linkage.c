#include "include/linkage.h"
#include "include/as_frontend.h"
#include "include/io.h"
#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *sh(const char *cmd) {
  char *output = (char *)calloc(1, sizeof(char));
  output[0] = '\0';

  FILE *fp;
  char path[1035];

  fp = popen(cmd, "r");

  if (fp == NULL) {
    printf("\e[31m[IO Error]: Failed to run command.\e[0m\n");
    exit(1);
  }

  while (fgets(path, sizeof(path), fp) != NULL) {
    output = (char *)realloc(output, (strlen(output) + strlen(path) + 1) *
                                         sizeof(char));
  }

  pclose(fp);

  return output;
}

void lk_compile(char *src) {
  lexer_T *lexer = init_lexer(src);
  parser_T *parser = init_parser(lexer);
  AST_T *root = parser_parse(parser);

  char *s = as_f_root(root);

  lk_write_file("a.s", s);
  sh("as a.s -o a.o");
  sh("ld a.o -o a");
}

void lk_compile_file(const char *filename) {
  char *src = lk_read_file(filename);
  lk_compile(src);
  free(src);
}
