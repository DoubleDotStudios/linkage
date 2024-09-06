#ifndef LK_AST_H
#define LK_AST_H
#include "list.h"

typedef struct AST {
  enum {
    AST_COMPOUND,
    AST_FN_DEF,
    AST_VAR,
    AST_STMT,
    AST_NOOP,
    AST_DEF_TYPE
  } type;
} AST_T;

AST_T *init_ast(int type);
#endif // !LK_AST_H
