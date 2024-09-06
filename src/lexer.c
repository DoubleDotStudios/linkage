#include "include/lexer.h"
#include "include/macros.h"
#include "include/token.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

lexer_T *init_lexer(char *src) {
  lexer_T *lexer = calloc(1, sizeof(struct LEXER));
  lexer->src = src;
  lexer->src_size = strlen(src);
  lexer->i = 0;
  lexer->c = src[lexer->i];

  return lexer;
}

void lexer_advance(lexer_T *lexer) {
  if (lexer->i < lexer->src_size && lexer->c != '\0') {
    lexer->i += 1;
    lexer->c = lexer->src[lexer->i];
  }
}

char lexer_peek(lexer_T *lexer, int offset) {
  return lexer->src[MIN(lexer->i + offset, lexer->src_size)];
}

token_T *lexer_advance_with(lexer_T *lexer, token_T *token) {
  lexer_advance(lexer);
  return token;
}

token_T *lexer_advance_current(lexer_T *lexer, int type) {
  char *val = calloc(2, sizeof(char));
  val[0] = lexer->c;
  val[1] = '\0';

  token_T *token = init_token(val, type);
  lexer_advance(lexer);

  return token;
}

void lexer_skip_whitespace(lexer_T *lexer) {
  while (lexer->c == 13 || lexer->c == 10 || lexer->c == ' ' ||
         lexer->c == '\t')
    lexer_advance(lexer);
}

token_T *lexer_parse_id(lexer_T *lexer) {
  int tk = TK_ID;

  char *val = calloc(1, sizeof(char));

  while (isalnum(lexer->c)) {
    val = realloc(val, (strlen(val) + 2) * sizeof(char));
    strcat(val, (char[]){lexer->c, 0});
    lexer_advance(lexer);
  }

  if (strcmp(val, "fn") == 0)
    tk = TK_FN;

  return init_token(val, tk);
}

token_T *lexer_parse_number(lexer_T *lexer) {
  char *val = calloc(1, sizeof(char));

  while (isdigit(lexer->c)) {
    val = realloc(val, (strlen(val) + 2) * sizeof(char));
    strcat(val, (char[]){lexer->c, 0});
    lexer_advance(lexer);
  }
  return init_token(val, TK_NUM);
}

token_T *lexer_next_token(lexer_T *lexer) {
  while (lexer->c != '\0') {
    lexer_skip_whitespace(lexer);

    if (isalpha(lexer->c))
      return lexer_parse_id(lexer);

    if (isdigit(lexer->c))
      return lexer_parse_number(lexer);

    switch (lexer->c) {
    case '=': {
      if (lexer_peek(lexer, 1) == '=')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token("==", TK_EQEQ)));
      return lexer_advance_with(lexer, init_token("=", TK_EQ));
    } break;
    case ';': {
      if (lexer_peek(lexer, 1) == ';')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token(";;", TK_CONST)));
      return lexer_advance_with(lexer, init_token(";", TK_VAR));
    }

    // Comparison
    case '>': {
      if (lexer_peek(lexer, 1) == '=')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token(">=", TK_GTE)));
      return lexer_advance_with(lexer, init_token(">", TK_GT));
    } break;
    case '<': {
      if (lexer_peek(lexer, 1) == '=')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token("<=", TK_LTE)));
      return lexer_advance_with(lexer, init_token("<", TK_LT));
    } break;
    case '!': {
      if (lexer_peek(lexer, 1) == '=')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token("!=", TK_NE)));
      return lexer_advance_with(lexer, init_token("!", TK_NOT));
    } break;

    // Binary Operation
    case '+': {
      if (lexer_peek(lexer, 1) == '+')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token("++", TK_INC)));
      return lexer_advance_with(lexer, init_token("+", TK_PLUS));
    } break;
    case '-': {
      if (lexer_peek(lexer, 1) == '-')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token("--", TK_DEC)));
      return lexer_advance_with(lexer, init_token("-", TK_MIN));
    } break;
    case '*':
      return lexer_advance_with(lexer, init_token("*", TK_MUL));
    case '/':
      return lexer_advance_with(lexer, init_token("/", TK_DIV));
    case '%':
      return lexer_advance_with(lexer, init_token("%", TK_MOD));
    case '^': {
      if (lexer_peek(lexer, 1) == '^')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token("^^", TK_SQR)));
      return lexer_advance_with(lexer, init_token("^", TK_POW));
    } break;
    case '#': {
      if (lexer_peek(lexer, 1) == '#')
        return lexer_advance_with(
            lexer, lexer_advance_with(lexer, init_token("##", TK_SQRT)));
      return lexer_advance_with(lexer, init_token("#", TK_ROOT));
    } break;

    // Brackets
    case '{':
      return lexer_advance_current(lexer, TK_LBRACE);
    case '}':
      return lexer_advance_current(lexer, TK_RBRACE);
    case '(':
      return lexer_advance_current(lexer, TK_LPAREN);
    case ')':
      return lexer_advance_current(lexer, TK_RPAREN);
    case '[':
      return lexer_advance_current(lexer, TK_RSQR);
    case ']':
      return lexer_advance_current(lexer, TK_LSQR);

    // Other
    case ',':
      return lexer_advance_with(lexer, init_token(",", TK_COMMA));
    case ':':
      return lexer_advance_with(lexer, init_token(":", TK_COLON));

    // Special characters
    case '\0':
      break;
    default:
      printf("\e[31m[Lexer Error]: Unable to convert %c to token.\e[0m\n",
             lexer->c);
      exit(1);
    }
  }
  return init_token(0, TK_EOF);
}
