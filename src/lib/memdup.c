#include <stdlib.h>
#include <string.h>

void* memdup
  (const void* mem, unsigned size)
{ 
  void* out = malloc(size);
  if(out != NULL) {
    memcpy(out, mem, size);
  }
  return out;
}
