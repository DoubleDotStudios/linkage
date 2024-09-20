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

char *as_f_var(AST_T *ast, int id) {
  char *s = calloc(1, sizeof(char));

  if (ast->type == AST_NUM) {
    const char *template = "  $%d";
    s = realloc(s, (strlen(template) + 256) * sizeof(char));
    sprintf(s, template, ast->int_val);
  } else {
    const char *template = "  %d(%%esp)";
    s = realloc(s, (strlen(template) + 4) * sizeof(char));
    sprintf(s, template, id);
  }
  return s;
}

char *as_f_fn_def(AST_T *ast) {
  const char *template = ".global %s\n"
                         "%s:\n"
                         "%s\n"
                         "  pushl %%ebp\n"
                         "  movl %%esp, %%ebp\n"
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

char *as_f_ret(AST_T *ast) {
  char *s = calloc(1, sizeof(char));

  char *var_s = calloc(3, sizeof(char));
  var_s[0] = '$';
  var_s[1] = '0';
  var_s[2] = '\0';

  if (ast->value) {
    char *as_var_s = as_f_var(ast->value, 8);
    var_s = realloc(var_s, (strlen(as_var_s) + 1) * sizeof(char));
    strcpy(var_s, as_var_s);
    free(as_var_s);
  }

  const char *template = "  movl %s, %%eax\n"
                         "  movl %%ebp, %%esp\n"
                         "  popl %%ebp\n"
                         "  ret\n";

  char *ret = calloc(strlen(template) + 128, sizeof(char));
  sprintf(ret, template, var_s);
  s = realloc(s, (strlen(ret) + 1) * sizeof(char));
  strcat(s, ret);

  return s;
}

char *as_f_root(AST_T *ast) {
  const char *section_text = ".section .text\n"
                             ".global _start\n"
                             "_start:\n"
                             "  pushl 0(\%esp)\n"
                             "  call main\n"
                             "  addl $4, \%esp\n"
                             "  movl \%eax, \%ebx\n"
                             "  movl $1, \%eax\n"
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
    next_val = as_f_var(ast, 0);
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
  case AST_RET:
    next_val = as_f_ret(ast);
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
