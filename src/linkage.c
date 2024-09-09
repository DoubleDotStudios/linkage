#include "include/linkage.h"
#include "include/as_frontend.h"
#include "include/io.h"
#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>

void lk_compile(char *src) {
  lexer_T *lexer = init_lexer(src);
  parser_T *parser = init_parser(lexer);
  AST_T *root = parser_parse(parser);

  char *s = as_f(root);

  printf("%s\n", s);
}

void lk_compile_file(const char *filename) {
  char *src = lk_read_file(filename);
  lk_compile(src);
  free(src);
}
