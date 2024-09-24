#ifndef LK_VISITOR_H
#define LK_VISITOR_H
#include "AST.h"
#include "list.h"

typedef struct VISITOR {
  AST_T *object;
} visitor_T;

visitor_T *init_visitor();

AST_T *visitor_visit(visitor_T *visitor, AST_T *node, list_T *list);

AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_var(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_def(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_fn_def(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_call(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_num(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_id(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_ret(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_access(visitor_T *visitor, AST_T *node, list_T *list);
AST_T *visitor_visit_args(visitor_T *visitor, AST_T *node, list_T *list);

#endif // !LK_VISITOR_H
