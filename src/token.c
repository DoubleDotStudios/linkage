#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_T *init_token(char *value, int type) {
  token_T *token = calloc(1, sizeof(struct TOKENS));
  token->value = value;
  token->type = type;

  return token;
}

char *tok_t_to_str(int type) {
  switch (type) {
  case TK_FN:
    return "TK_FN";
  case TK_ID:
    return "TK_ID";
  case TK_NUM:
    return "TK_NUM";
  case TK_STR:
    return "TK_STR";
  case TK_VAR:
    return "TK_VAR";
  case TK_CONST:
    return "TK_CONST";

  case TK_LPAREN:
    return "TK_LPAREN";
  case TK_RPAREN:
    return "TK_RPAREN";
  case TK_LBRACE:
    return "TK_LBRACE";
  case TK_RBRACE:
    return "TK_RBRACE";
  case TK_RSQR:
    return "TK_RSQR";
  case TK_LSQR:
    return "TK_LSQR";

  case TK_EQ:
    return "TK_EQ";
  case TK_COLON:
    return "TK_COLON";
  case TK_COMMA:
    return "TK_COMMA";
  case TK_PIPE:
    return "TK_PIPE";

  case TK_PLUS:
    return "TK_PLUS";
  case TK_MIN:
    return "TK_MIN";
  case TK_DIV:
    return "TK_DIV";
  case TK_MUL:
    return "TK_MUL";
  case TK_ROOT:
    return "TK_ROOT";
  case TK_MOD:
    return "TK_MOD";
  case TK_POW:
    return "TK_POW";

  case TK_SQRT:
    return "TK_SQRT";
  case TK_SQR:
    return "TK_SQR";
  case TK_INC:
    return "TK_INC";
  case TK_DEC:
    return "TK_DEC";

  case TK_GT:
    return "TK_GT";
  case TK_LT:
    return "TK_LT";
  case TK_NOT:
    return "TK_NOT";

  case TK_GTE:
    return "TK_GTE";
  case TK_LTE:
    return "TK_LTE";
  case TK_NE:
    return "TK_NE";
  case TK_EQEQ:
    return "TK_EQEQ";
  case TK_OR:
    return "TK_OR";

  case TK_EOF:
    return "TK_EOF";
  }

  return "Failed to convert token type to string.";
}

char *tok_to_str(token_T *token) {
  const char *type = tok_t_to_str(token->type);
  const char *templ = "\e[32mType: %s || Type ID: %d || Value: %s";

  char *str = calloc(strlen(type) + strlen(templ) + 8, sizeof(char));
  sprintf(str, templ, type, token->type, token->value);
  return str;
}
