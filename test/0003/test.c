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
    vec_appendstr(&vec, "123456789");
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_shift(&vec, 5, 3, -1);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (strcmp(vec.data, "1236789")) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    unsigned char cmp[] = { '1','2','3','4','5',0,0,'6','7','8','9' };
    vec_t vec = { 0 };
    vec_appendstr(&vec, "123456789");
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_shift(&vec, 5, 7, -1);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (memcmp(vec.data, cmp, sizeof(cmp))) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  fprintf(stderr, "SUCCESS\n");

  return 0;
}
