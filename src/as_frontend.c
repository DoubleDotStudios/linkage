#include "include/as_frontend.h"
#include "include/AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *as_f_compound(AST_T *ast) {
  char *val = calloc(1, sizeof(char));

  for (int i = 0; i < ast->children->size; i++) {
    AST_T *child = (AST_T *)ast->children->items[i];
    char *next_val = as_f(child);
    val = realloc(val, (strlen(val) + strlen(next_val) + 1) * sizeof(char));
    strcat(val, next_val);
  }

  return val;
}

char *as_f_var(AST_T *ast) {}

char *as_f_fn_def(AST_T *ast) {
  const char *template = ".global %s\n"
                         "%s:\n"
                         "%s\n"
                         "%s\n";

  char *args = as_f_args(ast->args);

  char *body = as_f_compound(ast->value);

  char *s = calloc((strlen(template) + (strlen(ast->name) * 2) + 1) +
                       strlen(args) + strlen(body),
                   sizeof(char));
  sprintf(s, template, ast->name, ast->name, args, body);

  return s;
}

char *as_f_fn_call(AST_T *ast) {
  char *s = calloc(1, sizeof(char));

  if (strcmp(ast->name, "return") == 0) {
    AST_T *arg1 = (AST_T *)ast->value->children->size
                      ? ast->value->children->items[0]
                      : (void *)0;
    const char *template = "  mov $%d, %%eax\n"
                           "  ret\n";

    char *ret = calloc(strlen(template) + 128, sizeof(char));
    sprintf(ret, template, arg1 ? arg1->int_val : 0);
    s = realloc(s, (strlen(ret) + 1) * sizeof(char));
    strcat(s, ret);
  }

  return s;
}

char *as_f_args(AST_T *ast) {
  char *val = calloc(1, sizeof(char));
  for (int i = 0; i < (int)ast->children->size; i++) {
    AST_T *child = (AST_T *)ast->children->items[i];
    char *next_val = as_f(child);
    val = realloc(val, (strlen(next_val) + 1) * sizeof(char));
    strcat(val, next_val);
  }

  return val;
}

char *as_f_num(AST_T *ast) {}

char *as_f_id(AST_T *ast) {
  const char *template = "  /*id %s*/\n";

  char *s = calloc((strlen(template) + strlen(ast->name) + 1), sizeof(char));
  sprintf(s, template, ast->name);

  return s;
}

char *as_f_root(AST_T *ast) {
  const char *section_text = ".section .text\n"
                             ".global _start\n"
                             "_start:\n"
                             "  call main\n"
                             "  mov \%eax, \%ebx\n"
                             "  mov $1, \%eax\n"
                             "  int $0x80\n\n";

  char *val = (char *)calloc((strlen(section_text) + 128), sizeof(char));
  strcpy(val, section_text);

  char *next_val = as_f(ast);

  val = realloc(val, (strlen(val) + strlen(next_val) + 1) * sizeof(char));
  strcat(val, next_val);

  return val;
}

char *as_f(AST_T *ast) {
  char *val = calloc(1, sizeof(char));
  char *next_val = 0;

  switch (ast->type) {
  case AST_COMPOUND:
    next_val = as_f_compound(ast);
    break;
  case AST_VAR_DEF:
    next_val = as_f_var(ast);
    break;
  case AST_FN_DEF:
    next_val = as_f_fn_def(ast);
    break;
  case AST_FN_CALL:
    next_val = as_f_fn_call(ast);
    break;
  case AST_FN_ARGS:
    next_val = as_f_compound(ast);
    break;
  case AST_NUM:
    next_val = as_f_compound(ast);
    break;
  case AST_ID:
    next_val = as_f_id(ast);
    break;
  default:
    printf(
        "\e[31m[Codegen Error]: Failed to generate asm for AST type %d.\e[0m\n",
        ast->type);
    exit(1);
    break;
  }

  val = realloc(val, (strlen(next_val) + 1) * sizeof(char));
  strcat(val, next_val);

  return val;
}
