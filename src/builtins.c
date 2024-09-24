#include "include/builtins.h"
#include "include/AST.h"
#include "include/list.h"
#include "include/utils.h"
#include "include/visitor.h"
#include <stdio.h>
#include <string.h>

AST_T *fptr_print(visitor_T *visitor, AST_T *node, list_T *list) {
  printf("her\n");
  AST_T *ast = init_ast(AST_STR);

  AST_T *arg = list->size ? (AST_T *)list->items[0] : (AST_T *)0;
  char *str = 0;

  char *hex = 0;

  if (arg) {
    if (arg->type == AST_STR)
      str = arg->string_val;
    else if (arg->type == AST_NUM) {
      str = calloc(128, sizeof(char));
      sprintf(str, "%d", arg->int_val);
    }
    hex = str_to_hex(str);
  }

  const char *template = "movl $4, %%eax\n"
                         "movl $1, %%ebx\n"
                         "pushl $0x%s\n"
                         "movl %%esp, %%ecx\n"
                         "movl $%d, %%edx\n"
                         "int $0x80\n";

  char *asmb =
      calloc((hex ? strlen(hex) : 0) + strlen(template) + 1, sizeof(char));
  sprintf(asmb, template, hex ? hex : "$0", 8);
  ast->string_val = asmb;
  free(hex);

  return ast;
}

void register_builtins_fptr(list_T *list, const char *name,
                            AST_T *(*fptr)(visitor_T *visitor, AST_T *node,
                                           list_T *list)) {
  AST_T *fptr_print_var = init_ast(AST_FN_CALL);
  fptr_print_var->name = mkstr(name);
  fptr_print_var->fptr = fptr;

  list_push(list, fptr_print_var);
}

void register_builtins(list_T *list) {
  register_builtins_fptr(list, "print", fptr_print);
}
