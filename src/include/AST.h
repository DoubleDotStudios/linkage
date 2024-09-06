#ifndef LK_AST_H
#define LK_AST_H
#include "list.h"

typedef struct AST {
  enum {
    AST_COMPOUND,
    AST_FN_DEF,
    AST_VAR_DEF,
    AST_CONST_DEF,
    AST_STMT,
    AST_NOOP,
    AST_DEF_TYPE,
    AST_VAR_CALL
  } type;

  list_T *children;
  char *name;
  struct AST *value;
} AST_T;

AST_T *init_ast(int type);
#endif // !LK_AST_H