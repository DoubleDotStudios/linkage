#include "include/builtins.h"
#include "include/AST.h"
#include "include/as_frontend.h"
#include "include/list.h"
#include "include/utils.h"
#include "include/visitor.h"
#include <stdio.h>
#include <string.h>

AST_T *fptr_print(visitor_T *visitor, AST_T *node, list_T *args) {
  AST_T *ast = init_ast(AST_STR);

  AST_T *arg = args->size ? (AST_T *)(AST_T *)args->items[0] : (AST_T *)0;
  char *str = arg ? arg->string_val : 0;
  unsigned int nchunks = 0;
  unsigned int nbytes = 0;
  char *hex = 0;

  if (arg) {
    if (arg->type == AST_NUM) {
      char *istr = calloc(128, sizeof(char));
      sprintf(istr, "%d", arg->int_val);
      str = istr;
    } else if (arg->type == AST_ID) {
      arg->type = AST_VAR_CALL;
      char *push = as_f_var(arg, args);
      hex = push;
    } else if (arg->type == AST_ACCESS) {
      char *pushstr = as_f(arg, args);
      hex = pushstr;

      const char *strlenas = "call strlen\n"
                             "popl \%esp\n";

      hex = realloc(hex, (strlen(hex) + strlen(strlenas) + 1) * sizeof(char));
      strcat(hex, strlenas);
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

  nbytes = nbytes ? nbytes : (nchunks * 4);
  char *sizeasstr = (char *)calloc(1, sizeof(char));

  if (nbytes) {
    const char *size_template = "movl $%d, %%edx\n";
    sizeasstr =
        realloc(sizeasstr, (strlen(size_template) + 128) * sizeof(char));
    sprintf(sizeasstr, size_template, nbytes);
  } else {
    const char *size_template = "movl %eax, %edx\n";
    sizeasstr =
        realloc(sizeasstr, (strlen(size_template) + 128) * sizeof(char));
    strcpy(sizeasstr, size_template);
  }

  const char *template = "%s\n"                // buffer
                         "movl %%esp, %%ecx\n" // buffer
                         "addl $%d, %%esp\n"
                         "%s\n"             // size
                         "movl $4, %%eax\n" // syscall write
                         "movl $1, %%ebx\n" // stdout
                         "int $0x80\n";

  char *asmb =
      calloc((hex ? strlen(hex) : 0) + strlen(template) + strlen(sizeasstr) + 1,
             sizeof(char));
  sprintf(asmb, template, hex ? hex : "$0", nbytes, sizeasstr);
  ast->string_val = asmb;
  free(hex);

  return ast;
}

void register_builtins_fptr(list_T *list, const char *name,
                            AST_T *(*fptr)(visitor_T *visitor, AST_T *node,
                                           list_T *args)) {
  AST_T *fptr_print_var = init_ast(AST_FN_CALL);
  fptr_print_var->name = mkstr(name);
  fptr_print_var->fptr = fptr;

  list_push(list, fptr_print_var);
}

void register_builtins(list_T *list) {
  register_builtins_fptr(list, "print", fptr_print);
}
