#include "include/linkage.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("\e[31mIncorrect usage of llsc. Correct usage is:\e[0m\n\e[34m  "
           "llsc <input>\e[0m\n");
    return 1;
  }

  lk_compile_file(argv[1]);

  return 0;
}
