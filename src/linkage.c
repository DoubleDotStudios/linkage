#include "include/linkage.h"
#include "include/io.h"
#include "include/parser.h"
#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>

void lk_compile(char *src) {
  lexer_T *lexer = init_lexer(src);
  parser_T *parser = init_parser(lexer);
  AST_T *root = parser_parse(parser);

  printf("%d\n", root->children->size);

  token_T *tok = 0;

  while ((tok = lexer_next_token(lexer))->type != TK_EOF) {
    printf("\e[32m%s\e[0m\n", tok_to_str(tok));
  }
}

void lk_compile_file(const char *filename) {
  char *src = lk_read_file(filename);
  lk_compile(src);
  free(src);
}
