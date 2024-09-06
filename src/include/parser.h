#ifndef LK_PARSER_H
#define LK_PARSER_H
#include "AST.h"
#include "lexer.h"

typedef struct PARSER {
  lexer_T *lexer;
  token_T *token;
} parser_T;

parser_T *init_parser(lexer_T *lexer);

token_T *parser_eat(parser_T *parser, int type);

AST_T *parser_parse(parser_T *parser);

AST_T *parser_parse_fn(parser_T *parser);

AST_T *parser_parse_var(parser_T *parser);

AST_T *parser_parse_id(parser_T *parser, int fn_name);

AST_T *parser_parse_num(parser_T *parser);

AST_T *parser_parse_block(parser_T *parser);

AST_T *parser_parse_args(parser_T *parser);

AST_T *parser_parse_paren(parser_T *parser);

AST_T *parser_parse_expr(parser_T *parser);

AST_T *parser_parse_compound(parser_T *parser);
#endif // !LK_PARSER_H
