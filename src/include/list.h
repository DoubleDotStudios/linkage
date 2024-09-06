#ifndef LK_LIST_H
#define LK_LIST_H
#include <stdlib.h>

typedef struct LIST {
  void **items;
  size_t size;
  size_t item_size;
} list_T;

list_T *init_list(size_t item_size);

void list_push(list_T *list, void *item);
#endif // !LK_LIST_H
