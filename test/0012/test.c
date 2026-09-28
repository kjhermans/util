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

  unsigned s = ulist_size(&l);
  fprintf(stderr, "Size of list = %u\n", s);

  ulist_t l1 = { 0 };
  fprintf(stderr, "Size of empty list = %u\n", ulist_size(&l1));

  ulist_push(&l1, 1);
  fprintf(stderr, "Size of list with one elt = %u\n", ulist_size(&l1));

  ulist_push(&l1, 1000);
  fprintf(stderr, "Size of list with two elts = %u\n", ulist_size(&l1));

  ulist_pop(&l1, NULL);
  fprintf(stderr, "Size of list with one elt = %u\n", ulist_size(&l1));

  ulist_pop(&l1, NULL);
  fprintf(stderr, "Size of empty list = %u\n", ulist_size(&l1));

  r = ulist_pop(&l1, NULL);
  fprintf(stderr, "Return code of last pop is %d\n", r);

  return 0;
}
