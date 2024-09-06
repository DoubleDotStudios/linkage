#include "include/AST.h"
#include "include/list.h"
#include <stdlib.h>

AST_T *init_ast(int type) {
  AST_T *ast = calloc(1, sizeof(struct AST));
  ast->type = type;

  if (type == AST_COMPOUND)
    ast->children = init_list(sizeof(struct AST *));

  return ast;
}
