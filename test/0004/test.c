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
    vec_ca_encode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (vec_ca_decode(&vec)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (strcmp(vec.data, "123")) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    vec_t vec_orig = { 0 };
    vec_t vec = { 0 };
    for (unsigned i=0; i < 200; i++) {
      vec_appendstr(&vec_orig, "12345");
    }
    vec_copy(&vec, &vec_orig);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_ca_encode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (vec_ca_decode(&vec)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (vec_compare(&vec, &vec_orig)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    vec_t vec_orig = { 0 };
    vec_t vec = { 0 };
    for (unsigned i=0; i < 200; i++) {
      vec_appendstr(&vec_orig, "12345");
    }
    vec_copy(&vec, &vec_orig);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_ca_encode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec.data[ 357 ] = 'x'; /* Introducing the error to be corrected */
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (vec_ca_decode(&vec)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (vec_compare(&vec, &vec_orig)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    vec_t vec_orig = { 0 };
    vec_t vec = { 0 };
    for (unsigned i=0; i < 200; i++) {
      vec_appendstr(&vec_orig, "12345");
    }
    vec_copy(&vec, &vec_orig);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec_ca_encode(&vec);
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    vec.data[ 357 ] = 'x'; /* Introducing the error to be corrected */
    vec.data[ 359 ] = 'y'; /* Introducing the error to be corrected */
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
    if (vec_ca_decode(&vec)) {
      fprintf(stderr, "Double error cannot be corrected as expected.\n");
    }
    fprintf(stderr, "VEC: %s (%u)\n", vec.data, vec.size);
  }
  fprintf(stderr, "Subtest Ok.\n");

  fprintf(stderr, "SUCCESS\n");

  return 0;
}
