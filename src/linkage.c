#include "include/linkage.h"
#include "include/AST.h"
#include "include/as_frontend.h"
#include "include/io.h"
#include "include/list.h"
#include "include/parser.h"
#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *sh(const char *cmd) {
  char *output = (char *)calloc(1, sizeof(char));
  output[0] = '\0';

  FILE *fp;
  char path[1035];

  fp = popen(cmd, "r");

  if (fp == NULL) {
    printf("\e[31m[IO Error]: Failed to run command.\e[0m\n");
    exit(1);
  }

  while (fgets(path, sizeof(path), fp) != NULL) {
    output = (char *)realloc(output, (strlen(output) + strlen(path) + 1) *
                                         sizeof(char));
  }

  pclose(fp);

  return output;
}

void lk_compile(char *src, const char *name) {
  lexer_T *lexer = init_lexer(src);
  parser_T *parser = init_parser(lexer);
  AST_T *root = parser_parse(parser);

  visitor_T *visitor = init_visitor();
  AST_T *opt_root =
      visitor_visit(visitor, root, init_list(sizeof(struct AST *)));

  char *s = as_f_root(opt_root, init_list(sizeof(struct AST *)));

  char *n = strrchr(name, '/') + 1;
  n = strtok(n, ".");

  char *as = "%s.s";
  char *as_s = calloc(strlen(as) + strlen(n), sizeof(char));
  sprintf(as_s, as, n);

  lk_write_file(as_s, s); // Create asm file

  as = "as --32 %s.s -o %s.o";
  as_s = realloc(as_s, (strlen(as) + (strlen(n) * 2) * sizeof(char)));
  sprintf(as_s, as, n, n);

  sh(as_s); // Create object file

  as = "ld %s.o -o %s -m elf_i386";
  as_s = realloc(as_s, (strlen(as) + (strlen(n) * 2) * sizeof(char)));
  sprintf(as_s, as, n, n);

  sh(as_s); // Create executable
}

void lk_compile_file(const char *filename) {
  char *src = lk_read_file(filename);
  lk_compile(src, filename);
  free(src);
}
