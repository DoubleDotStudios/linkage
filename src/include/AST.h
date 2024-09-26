#ifndef LK_AST_H
#define LK_AST_H
#include "list.h"

struct VISITOR;

typedef struct AST {
  enum {
    AST_COMPOUND,
    AST_FN_DEF,
    AST_VAR_DEF,
    AST_CONST_DEF,
    AST_STMT,
    AST_NOOP,
    AST_DEF_TYPE,
    AST_ID,
    AST_RET,
    AST_FN_CALL,
    AST_VAR_CALL,
    AST_NUM,
    AST_ACCESS,
    AST_FN_ARGS,
    AST_STR,
    AST_LANG
  } type;

  list_T *children;
  char *name;
  char *string_val;
  struct AST *value;
  int int_val;
  int data_type;
  int id;
  struct AST *args;
  struct AST *(*fptr)(struct VISITOR *visitor, struct AST *node,
                      struct LIST *list);
} AST_T;

AST_T *init_ast(int type);
#endif // !LK_AST_H
