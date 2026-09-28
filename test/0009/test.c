#include <stdio.h>
#include <util/hash.h>

/**
 *
 */
int main
  (int argc, char* argv[])
{
  hash_t h = { 0 };
  vec_t key = vec_string("Foo");
  vec_t value = vec_string("Bar");
  vec_t get = { 0 };

  hash_init(&h);
  int p = hash_put(&h, &key, &value); (void)p;
  if (hash_get(&h, &key, &get) == 0) {
    if (0 == vec_compare(&value, &get)) {
      fprintf(stderr, "Retrieval returns ok.\n");
      free(key.data);
      free(value.data);
      hash_free(&h);
      return 0;
    } else {
      fprintf(stderr, "Retrieval returns '%-.*s'\n", get.size, get.data);
    }
  } else {
    fprintf(stderr, "Retrieval returns non zero.\n");
  }
  return ~0;
}
