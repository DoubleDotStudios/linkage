#ifndef LK_TOKEN_H
#define LK_TOKEN_H
typedef struct TOKENS {
  char *value;
  enum {
    TK_FN,    //
    TK_RET,   //
    TK_ID,    //
    TK_NUM,   //
    TK_VAR,   //
    TK_CONST, //

    TK_LPAREN, //
    TK_RPAREN, //
    TK_LBRACE, //
    TK_RBRACE, //
    TK_RSQR,   //
    TK_LSQR,   //

    TK_EQ,    //
    TK_COLON, //
    TK_COMMA, //
    TK_TYPE,  //

    TK_PLUS, //
    TK_MIN,  //
    TK_DIV,  //
    TK_MUL,  //
    TK_ROOT, //
    TK_MOD,  //
    TK_POW,  //

    TK_SQRT, //
    TK_SQR,  //
    TK_INC,  //
    TK_DEC,  //

    TK_GT,  //
    TK_LT,  //
    TK_NOT, //

    TK_GTE,  //
    TK_LTE,  //
    TK_NE,   //
    TK_EQEQ, //

    TK_EOF //
  } type;
} token_T;

token_T *init_token(char *value, int type);

char *tok_t_to_str(int type);

char *tok_to_str(token_T *token);
#endif // !LK_TOKEN_H
