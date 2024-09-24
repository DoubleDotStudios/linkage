#ifndef LK_AS_FRONTEND_H
#define LK_AS_FRONTEND_H
#include "AST.h"
#include "list.h"
char *as_f(AST_T *ast, list_T *list);
char *as_fn_def(AST_T *ast, list_T *list);
char *as_f_args(AST_T *ast, list_T *list);
char *as_fn_call(AST_T *ast, list_T *list);
char *as_f_num(AST_T *ast, list_T *list);
char *as_f_str(AST_T *ast, list_T *list);
char *as_f_id(AST_T *ast, list_T *list);
char *as_f_access(AST_T *ast, list_T *list);
char *as_f_list(AST_T *ast, int id, list_T *list);
char *as_f_root(AST_T *ast, list_T *list);
#endif // !LK_AS_FRONTEND_H
