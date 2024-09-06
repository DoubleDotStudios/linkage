#include "include/AST.h"
#include <stdlib.h>

AST_T *init_ast(int type) {
  AST_T *ast = calloc(1, sizeof(struct AST));
  ast->type = type;

  return ast;
}
