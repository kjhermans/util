#include <string.h>
#include <stdio.h>
#include <vec_t.h>

int main
  (int argc, char* argv[])
{
  (void)argc;
  (void)argv;

  {
    vec_t vec = { 0 };
    vec_appendstr(&vec, "123");
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_hex_encode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_hex_decode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (strcmp(vec.data, "123")) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    vec_t vec = { 0 };
    vec_appendstr(&vec, "1234");
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_hex_encode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_hex_decode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (strcmp(vec.data, "1234")) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    vec_t vec = { 0 };
    vec_appendstr(&vec, "12345");
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_hex_encode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_hex_decode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (strcmp(vec.data, "12345")) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  fprintf(stderr, "SUCCESS\n");

  return 0;
}
