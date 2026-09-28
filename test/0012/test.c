#include <stdio.h>
#include <util/linkedlist.h>

MAKE_LLIST_HEADER(unsigned, ulist_)
MAKE_LLIST_CODE(unsigned, ulist_)

static
int iterator
  (ulist_t* list, unsigned index, unsigned* value, void* arg)
{
  fprintf(stderr, "Iterator: %u -> %u (%s)\n", index, *value, (char*)arg);
  return 0;
}

int main
  (int argc, char* argv[])
{
  ulist_t l = { 0 };

  ulist_push(&l, 2);
  ulist_push(&l, 3);
  ulist_push(&l, 5);
  ulist_push(&l, 7);
  ulist_push(&l, 11);
  ulist_push(&l, 13);
  ulist_push(&l, 17);
  ulist_push(&l, 19);

  ulist_t* node = &l;

  if (node->next) {
    do {
      fprintf(stderr, "Number: %u\n", node->value);
    } while ((node = node->next) != &l);
  }

  int r = ulist_iterate(&l, iterator, argv[0]);

  return 0;
}
