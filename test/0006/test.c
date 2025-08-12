#include <string.h>
#include <stdio.h>
#include <time.h>

#include <util/util.h>

int main
  (int argc, char* argv[])
{
  (void)argc;
  (void)argv;

  {
    char input[] = "1234567";
    unsigned l = sizeof(input)-1;
    char output[ l * 2 ];
    char base64[ l * 2 ];
    char* dummy;
    (void)dummy;

    fprintf(stderr, "Testing %u bytes.\n", l);
    dummy = b64_encode(input, l, base64);
    fprintf(stderr, "'%s' -> '%s'\n", input, base64);
    dummy = b64_decode(base64, output);
    if (0 != memcmp(input, output, l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    char input[] = "12345678";
    unsigned l = sizeof(input)-1;
    char output[ l * 2 ];
    char base64[ l * 2 ];
    char* dummy;
    (void)dummy;

    fprintf(stderr, "Testing %u bytes.\n", l);
    dummy = b64_encode(input, l, base64);
    fprintf(stderr, "'%s' -> '%s'\n", input, base64);
    dummy = b64_decode(base64, output);
    if (0 != memcmp(input, output, l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    char input[] = "123456789";
    unsigned l = sizeof(input)-1;
    char output[ l * 2 ];
    char base64[ l * 2 ];
    char* dummy;
    (void)dummy;

    fprintf(stderr, "Testing %u bytes.\n", l);
    dummy = b64_encode(input, l, base64);
    fprintf(stderr, "'%s' -> '%s'\n", input, base64);
    dummy = b64_decode(base64, output);
    if (0 != memcmp(input, output, l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    char input[] =
      "1234567890123456789012345678901234567890"
      "1234567890123456789012345678901234567890"
      "1234567890123456789012345678901234567890"
      "1234567890123456789012345678901234567890"
      "1234567890123456789012345678901234567890"
      "1234567890123456789012345678901234567890"
      "1234567890123456789012345678901234567890"
      "1234567890123456789012345678901234567890";
    unsigned l = sizeof(input)-1;
    char output[ l * 2 ];
    char base64[ l * 2 ];
    char* dummy;
    (void)dummy;

    fprintf(stderr, "Testing %u bytes.\n", l);
    dummy = b64_encode_spaced(input, l, base64);
    fprintf(stderr, "'%s' -> '%s'\n", input, base64);
    dummy = b64_decode(base64, output);
    if (0 != memcmp(input, output, l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    char input[ 4096 ] = { 0 };
    unsigned l = sizeof(input)-1;
    char output[ l * 2 ];
    char base64[ l * 2 ];
    char* dummy;
    (void)dummy;

    srand(time(0));
    for (unsigned i=0; i < sizeof(input)-1; i++) {
      input[ i ] = rand() % 256;
    }

    fprintf(stderr, "Testing %u bytes.\n", l);
    dummy = b64_encode_spaced(input, l, base64);
    fprintf(stderr, "'%s' -> '%s'\n", input, base64);
    dummy = b64_decode(base64, output);
    if (0 != memcmp(input, output, l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  fprintf(stderr, "SUCCESS\n");

  return 0;
}
