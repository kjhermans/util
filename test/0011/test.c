#include <stdio.h>
#include <util/hash.h>
#include <util/vec_t.h>

/**
 *
 */
int main
  (int argc, char* argv[])
{
  hash_t h = { 0 };
  vec_t get = { 0 };

  hash_init(&h);

#include "data.inc"

  put_data();
  get_data();
  return 0;
}
