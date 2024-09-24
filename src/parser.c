#include "include/parser.h"
#include "include/AST.h"
#include "include/lexer.h"
#include "include/list.h"
#include "include/token.h"
#include "include/types.h"
#include "include/utils.h"
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
  parser_eat(parser, TK_FN);

  AST_T *ast = init_ast(AST_FN_DEF);
  AST_T *id = parser_parse_id(parser, 1);
  char *name = id->name;

  if (name == NULL) {
    printf("\e[31m[Parser Error]: Expected identifier as function name.\n  "
           "Found %s instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  ast->args = parser_parse_args(parser);

  ast->value = parser_parse_compound(parser);

  ast->name = name;
  return ast;
}

AST_T *parser_parse_var(parser_T *parser) {
  parser_eat(parser, TK_VAR);

  AST_T *id = parser_parse_id(parser, 0);
  char *name = id->name;

  if (name == NULL) {
    printf("\e[31m[Parser Error]: Expected identifier in variable "
           "declaration.\n  Found %s instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  AST_T *ast = init_ast(AST_VAR_DEF);
  ast->name = name;
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

  return ast;
}

AST_T *parser_parse_block(parser_T *parser) {
  parser_eat(parser, TK_LBRACE);
  AST_T *ast = init_ast(AST_COMPOUND);

  while (parser->token->type != TK_RBRACE) {
    list_push(ast->children, parser_parse_expr(parser));
  }

  if (parser->token->type != TK_RBRACE) {
    printf("\e[31m[Parser Error]: Expected closing brace after function "
           "body.\n  Found %s instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  parser_eat(parser, TK_RBRACE);

  return ast;
}

AST_T *parser_parse_num(parser_T *parser) {
  int int_val = atoi(parser->token->value);
  parser_eat(parser, TK_NUM);

  AST_T *ast = init_ast(AST_NUM);
  ast->int_val = int_val;

  return ast;
}

AST_T *parser_parse_str(parser_T *parser) {
  char *str_val = mkstr(parser->token->value);
  parser_eat(parser, TK_STR);

  AST_T *ast = init_ast(AST_STR);
  ast->string_val = str_val;

  return ast;
}

AST_T *parser_parse_id(parser_T *parser, int fn_name) {
  if (parser->token->type != TK_ID) {
    printf("\e[31m[Parser Error]: Expected identifier but found token %s "
           "instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  char *value = calloc(strlen(parser->token->value) + 1, sizeof(char));
  strcpy(value, parser->token->value);
  parser_eat(parser, TK_ID);

  AST_T *ast = init_ast(AST_ID);
  ast->name = value;

  if (parser->token->type == TK_COLON) {
    parser_eat(parser, TK_COLON);

    while (parser->token->type == TK_ID) {
      ast->data_type = typename_to_int(parser->token->value);

      parser_eat(parser, TK_ID);

      if (parser->token->type == TK_LT) {
        parser_eat(parser, TK_LT);
        ast->data_type += typename_to_int(parser->token->value);
        parser_eat(parser, TK_ID);
        parser_eat(parser, TK_GT);
      }
    }
  } else {
    if (parser->token->type == TK_LPAREN && fn_name != 1) {
      ast->type = AST_FN_CALL;
      ast->value = parser_parse_args(parser);
    } else if (parser->token->type == TK_LSQR) {
      ast->type = AST_ACCESS;
      ast->value = parser_parse_list(parser);
    }
  }
  return ast;
}

AST_T *parser_parse_args(parser_T *parser) {
  parser_eat(parser, TK_LPAREN);

  AST_T *ast = init_ast(AST_FN_ARGS);

  if (parser->token->type != TK_RPAREN)
    list_push(ast->children, parser_parse_expr(parser));

  while (parser->token->type == TK_COMMA) {
    parser_eat(parser, TK_COMMA);
    list_push(ast->children, parser_parse_expr(parser));
  }

  if (parser->token->type != TK_RPAREN) {
    printf("\e[31m[Parser Error]: Expected closing parenthesis after opening "
           "parenthesis.\n  Found %s instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  parser_eat(parser, TK_RPAREN);

  if (parser->token->type == TK_COLON) {
    parser_eat(parser, TK_COLON);

    while (parser->token->type == TK_ID) {
      ast->data_type = typename_to_int(parser->token->value);

      parser_eat(parser, TK_ID);

      if (parser->token->type == TK_LT) {
        parser_eat(parser, TK_LT);
        ast->data_type += typename_to_int(parser->token->value);
        parser_eat(parser, TK_ID);
        parser_eat(parser, TK_GT);
      }
    }
  }

  return ast;
}

AST_T *parser_parse_ret(parser_T *parser) {
  AST_T *ast = init_ast(AST_RET);

  parser_eat(parser, TK_RET);

  if (parser->token->type == TK_NUM)
    ast->value = parser_parse_num(parser);
  else
    ast->value = parser_parse_id(parser, 0);

  return ast;
}

AST_T *parser_parse_paren(parser_T *parser) {
  AST_T *ast = init_ast(AST_COMPOUND);

  return ast;
}

AST_T *parser_parse_list(parser_T *parser) {
  parser_eat(parser, TK_LSQR);

  AST_T *ast = init_ast(AST_COMPOUND);

  if (parser->token->type != TK_RSQR)
    list_push(ast->children, parser_parse_expr(parser));

  while (parser->token->type == TK_COMMA) {
    parser_eat(parser, TK_COMMA);
    list_push(ast->children, parser_parse_expr(parser));
  }

  if (parser->token->type != TK_RSQR) {
    printf("\e[31m[Parser Error]: Expected closing parenthesis after opening "
           "parenthesis.\n  Found %s instead.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  }

  parser_eat(parser, TK_RSQR);

  return ast;
}

AST_T *parser_parse_pipe(parser_T *parser) {
  parser_eat(parser, TK_PIPE);
  AST_T *ast = init_ast(AST_NOOP);

  if (parser->token->type == TK_ID) {
    AST_T *ast = init_ast(AST_LANG);

    ast->name = parser_parse_id(parser, 0)->name;
    parser_eat(parser, TK_PIPE);
    parser_eat(parser, TK_LBRACE);
    // ast->value = parser_parse_foreign(parser);
    parser_eat(parser, TK_LBRACE);
  }

  return ast;
}

AST_T *parser_parse_expr(parser_T *parser) {
  switch (parser->token->type) {
  case TK_ID:
    return parser_parse_id(parser, 0);
  case TK_NUM:
    return parser_parse_num(parser);
  case TK_FN:
    return parser_parse_fn(parser);
  case TK_VAR:
    return parser_parse_var(parser);
  case TK_LPAREN:
    return parser_parse_paren(parser);
  case TK_RET:
    return parser_parse_ret(parser);
  case TK_LSQR:
    return parser_parse_list(parser);
  case TK_PIPE:
    return parser_parse_pipe(parser);
  case TK_STR:
    return parser_parse_str(parser);
  default: {
    printf("\e[31m[Parser Error]: Unexpected token %s.\e[0m\n",
           tok_t_to_str(parser->token->type));
    exit(1);
  };
  }
}

AST_T *parser_parse_compound(parser_T *parser) {
  unsigned int should_close = 0;

  if (parser->token->type == TK_LBRACE) {
    parser_eat(parser, TK_LBRACE);
    should_close = 1;
  }

  AST_T *compound = init_ast(AST_COMPOUND);

  while (parser->token->type != TK_EOF && parser->token->type != TK_RBRACE) {
    list_push(compound->children, parser_parse_expr(parser));
  }

  if (should_close == 1) {
    parser_eat(parser, TK_RBRACE);
    should_close = 0;
  }

  return compound;
}
