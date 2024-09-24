#ifndef LK_BUILTINS_H
#define LK_BUILTINS_H
#include "list.h"
#include "visitor.h"

AST_T *fptr_print(visitor_T *visitor, AST_T *node, list_T *list);
void register_builtins(list_T *list);

#endif // !LK_BUILTINS_H
