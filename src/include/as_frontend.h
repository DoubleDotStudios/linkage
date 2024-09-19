#ifndef LK_AS_FRONTEND_H
#define LK_AS_FRONTEND_H
#include "AST.h"
char *as_f(AST_T *ast);
char *as_fn_def(AST_T *ast);
char *as_f_args(AST_T *ast);
char *as_fn_call(AST_T *ast);
char *as_f_num(AST_T *ast);
char *as_f_id(AST_T *ast);
char *as_f_root(AST_T *ast);
#endif // !LK_AS_FRONTEND_H
