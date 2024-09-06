#include "include/parser.h"
#include "include/AST.h"
#include "include/lexer.h"
#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

parser_T *init_parser(lexer_T *lexer) {
  parser_T *parser = calloc(1, sizeof(struct PARSER));
  parser->lexer = lexer;
  parser->token = lexer_next_token(lexer);

  return parser;
}

token_T *parser_eat(parser_T *parser, int type) {
  if (parser->token->type != type) {
    printf("\e[31m[Parser Error]: Unexpected token %s\n  Expected %s\e[0m\n",
           tok_t_to_str(parser->token->type), tok_t_to_str(type));
    exit(1);
  }

  parser->token = lexer_next_token(parser->lexer);
  return parser->token;
}

AST_T *parser_parse(parser_T *parser) { return parser_parse_compound(parser); }

AST_T *parser_parse_fn(parser_T *parser) {
  parser_eat(parser, TK_FN); // Consume 'fn' keyword

  char *name = parser_parse_id(parser); // Parse the identifier
  if (name == NULL) {
    // If parsing fails or the name is invalid, handle the error
    printf("\e[31m[Parser Error]: Expected identifier as function name.\n  "
           "Found %s instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  printf("--> fn %s\n", name);

  // Proceed with AST creation
  AST_T *ast =
      init_ast(AST_FN_DEF); // Initialize the AST for the function definition
  ast->name = name;         // Set the function name
  return ast;
}

AST_T *parser_parse_var(parser_T *parser) {
  parser_eat(parser, TK_VAR); // Consume 'fn' keyword

  char *name = parser_parse_id(parser); // Parse the identifier
  if (name == NULL) {
    // If parsing fails or the name is invalid, handle the error
    printf("\e[31m[Parser Error]: Expected identifier in variable "
           "declaration.\n  Found %s instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  // Proceed with AST creation
  AST_T *ast =
      init_ast(AST_VAR_DEF); // Initialize the AST for the function definition
  ast->name = name;          // Set the function name
  if (parser->token->type == TK_EQ) {
    parser_eat(parser, TK_EQ);
    AST_T *value = parser_parse_expr(parser);
    if (value != NULL)
      ast->value = value;
    else
      printf("\e[31m[Parser Error]: Expected value when declaring a "
             "variable.\e[0m\n");
  } else {
    ast->value = 0;
  }

  printf("--> var %s = %p\n", name, ast->value);

  return ast;
}

char *parser_parse_id(parser_T *parser) {
  if (parser->token->type != TK_ID) {
    printf("\e[31m[Parser Error]: Expected identifier but found token %s "
           "instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    return NULL;
  }
  char *value = calloc(strlen(parser->token->value) + 1, sizeof(char));
  strcpy(value, parser->token->value);
  parser_eat(parser, TK_ID);

  return value;
}

AST_T *parser_parse_var_call(parser_T *parser) {
  if (parser->token->type != TK_ID) {
    printf("\e[31m[Parser Error]: Expected identifier but found token %s "
           "instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }
  char *value = calloc(strlen(parser->token->value) + 1, sizeof(char));
  strcpy(value, parser->token->value);
  parser_eat(parser, TK_ID);

  AST_T *ast = init_ast(AST_VAR_CALL);
  ast->name = value;
  return ast;
}

AST_T *parser_parse_expr(parser_T *parser) {
  switch (parser->token->type) {
  case TK_ID:
    parser_parse_var_call(parser);
  case TK_FN:
    parser_parse_fn(parser);
  case TK_VAR:
    parser_parse_var(parser);
  default: {
    printf("\e[31m[Parser Error]: Unexpected token %s.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  };
  }
}

AST_T *parser_parse_compound(parser_T *parser) {
  AST_T *compound = init_ast(AST_COMPOUND);
  while (parser->token->type != TK_EOF) {
    list_push(compound->children, parser_parse_expr(parser));
  }

  return compound;
}