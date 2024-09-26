#include "include/builtins.h"
#include "include/AST.h"
#include "include/list.h"
#include "include/utils.h"
#include "include/visitor.h"
#include <stdio.h>
#include <string.h>

AST_T *fptr_print(visitor_T *visitor, AST_T *node, list_T *list) {
  AST_T *ast = init_ast(AST_STR);

  AST_T *arg = list->size ? (AST_T *)(AST_T *)list->items[0] : (AST_T *)0;
  char *str = arg ? arg->string_val : 0;
  unsigned int nchunks = 0;
  char *hex = 0;

  if (arg) {
    if (arg->type == AST_NUM) {
      str = calloc(128, sizeof(char));
      sprintf(str, "%d\n", arg->int_val);
    } else if (arg->type == AST_VAR_CALL) {
      return arg;
    } else if (arg->type == AST_ACCESS) {
      return arg;
    } else if (arg->type == AST_STR) {
      str = arg->string_val;
    }

    if (str) {
      list_T *chunks = str_to_hex_chunks(str);
      nchunks = chunks->size;

      char *strpush = calloc(1, sizeof(char));
      const char *pushtemplate = "pushl $0x%s\n";

      for (unsigned int i = 0; i < chunks->size; i++) {
        char *pushex = (char *)chunks->items[(chunks->size - i) - 1];
        char *push =
            calloc(strlen(pushex) + strlen(pushtemplate) + 1, sizeof(char));
        sprintf(push, pushtemplate, pushex);
        strpush = realloc(strpush,
                          (strlen(strpush) + strlen(push) + 1) * sizeof(char));
        strcat(strpush, push);
        free(push);
      }

      hex = strpush;
    }
  }

  const char *template = "movl $4, %%eax\n"
                         "movl $1, %%ebx\n"
                         "%s"
                         "movl %%esp, %%ecx\n"
                         "addl $%d, %%esp\n"
                         "movl $%d, %%edx\n"
                         "int $0x80\n";

  char *asmb =
      calloc((hex ? strlen(hex) : 0) + strlen(template) + 1, sizeof(char));
  sprintf(asmb, template, hex ? hex : "pushl $0\n", nchunks * 4, nchunks * 4);
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
