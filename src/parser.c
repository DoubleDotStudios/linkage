#include "include/parser.h"
#include "include/AST.h"
#include "include/lexer.h"
#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>

parser_T *init_parser(lexer_T *lexer) {
  parser_T *parser = calloc(1, sizeof(struct PARSER));
  parser->lexer = lexer;
  parser->token = lexer_next_token(lexer);

  return parser;
}

token_T *parser_eat(parser_T *parser, int type) {
  if (parser->token->type != type) {
    printf("\e[31m[Parser Error]: Unexpected token %s\nExpected %s\e[0m\n",
           tok_to_str(parser->token), tok_t_to_str(type));
    exit(1);
  }

  parser->token = lexer_next_token(parser->lexer);
  return parser->token;
}

AST_T *parser_parse(parser_T *parser) {
  switch (parser->token->type) {}
}

AST_T *parser_parse_compound(parser_T *parser) {
  AST_T *compound = init_ast(AST_COMPOUND);
  while (parser->token->type != TK_EOF) {
    AST_T *child = parser_parse(parser);
  }
}
