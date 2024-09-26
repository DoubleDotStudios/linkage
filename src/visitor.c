#include "include/visitor.h"
#include "include/AST.h"
#include "include/builtins.h"
#include "include/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static AST_T *var_lookup(char *name, list_T *list) {
  for (int i = 0; i < (int)list->size; i++) {
    AST_T *ast = (AST_T *)list->items[i];

    if (!ast->name)
      continue;

    if (strcmp(ast->name, name) == 0)
      return ast;
  }

  return 0;
}

visitor_T *init_visitor() {
  visitor_T *visitor = calloc(1, sizeof(struct VISITOR));
  visitor->object = init_ast(AST_COMPOUND);

  register_builtins(visitor->object->children);

  return visitor;
}

AST_T *visitor_visit(visitor_T *visitor, AST_T *node, list_T *list) {
  switch (node->type) {
  case AST_STR:
    return node;
  case AST_COMPOUND:
    return visitor_visit_compound(visitor, node, list);
    break;
  case AST_VAR_CALL:
    return visitor_visit_var(visitor, node, list);
    break;
  case AST_VAR_DEF:
    return visitor_visit_def(visitor, node, list);
    break;
  case AST_FN_DEF:
    return visitor_visit_fn_def(visitor, node, list);
    break;
  case AST_FN_CALL:
    return visitor_visit_call(visitor, node, list);
    break;
  case AST_FN_ARGS:
    return visitor_visit_compound(visitor, node, list);
    break;
  case AST_NUM:
    // return visitor_visit_num(visitor, node, list);
    return node;
    break;
  case AST_ID:
    return visitor_visit_id(visitor, node, list);
    break;
  case AST_RET:
    // return visitor_visit_ret(visitor, node, list);
    return node;
    break;
  case AST_ACCESS:
    return visitor_visit_access(visitor, node, list);
    break;
  default:
    printf("\e[31m[Visitor]: Unable to handle AST type %d.\e[0m\n", node->type);
    exit(1);
    break;
  }

  return node;
}

AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node, list_T *list) {
  AST_T *compound = init_ast(AST_COMPOUND);

  for (unsigned int i = 0; i < node->children->size; i++)
    list_push(compound->children,
              visitor_visit(visitor, (AST_T *)node->children->items[i], list));

  return compound;
}

AST_T *visitor_visit_var(visitor_T *visitor, AST_T *node, list_T *list) {
  AST_T *var = var_lookup(node->name, visitor->object->children);

  if (var)
    return var;

  return node;
}

AST_T *visitor_visit_def(visitor_T *visitor, AST_T *node, list_T *list) {
  AST_T *var = init_ast(AST_VAR_DEF);

  var->name = node->name;
  var->value = visitor_visit(visitor, node->value, list);

  list_push(list, var);

  return var;
}

AST_T *visitor_visit_fn_def(visitor_T *visitor, AST_T *node, list_T *list) {
  AST_T *fn = init_ast(AST_FN_DEF);

  fn->name = node->name;

  fn->args = node->args;

  // fn->args = visitor_visit(visitor, node->args, list);

  fn->value = visitor_visit(visitor, node->value, node->value->children);

  return fn;
}

AST_T *visitor_visit_call(visitor_T *visitor, AST_T *node, list_T *list) {
  AST_T *var = var_lookup(node->name, visitor->object->children);

  if (var) {
    if (var->fptr) {
      return var->fptr(visitor, var, node->value->children);
    }
  }

  return node;
}

AST_T *visitor_visit_num(visitor_T *visitor, AST_T *node, list_T *list) {}

AST_T *visitor_visit_id(visitor_T *visitor, AST_T *node, list_T *list) {}

AST_T *visitor_visit_ret(visitor_T *visitor, AST_T *node, list_T *list) {}

AST_T *visitor_visit_access(visitor_T *visitor, AST_T *node, list_T *list) {
  int id = 0;

  for (unsigned int i = 0; i < list->size; i++) {
    if (strcmp(((AST_T *)list->items[i])->name, node->name) == 0) {
      id = i;
      break;
    }
  }

  node->id = id;

  return node;
}
