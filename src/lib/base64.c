/*
** Copyright 2001-2006 K.J.Hermans (kees@pink-frog.com)
** This code is part of ufy, a scripting language, and is licensed
** under the General Public License (http://www.gnu.org/licenses/gpl.txt)
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

int b64_encodedlength(int l) {
  return (((l+2)/3)*4)+1;
  // the last '1' is for '\0'
}

char* b64_encode(void* _data, int datalength, char* out) {
  unsigned char* data = _data;
  char alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
  int i, index;
  for (i=0, index=0; i<datalength; i+=3, index+=4) {
    int quad = 0;
    int trip = 0;
    int val = (0xFF & (int) data[i]);
    val <<= 8;
    if ((i+1) < datalength) {
      val |= (0xFF & (int) data[i+1]);
      trip = 1;
    }
    val <<= 8;
    if ((i+2) < datalength) {
      val |= (0xFF & (int) data[i+2]);
      quad = 1;
    }
    out[index+3] = alphabet[(quad? (val & 0x3F): 64)];
    val >>= 6;
    out[index+2] = alphabet[(trip? (val & 0x3F): 64)];
    val >>= 6;
    out[index+1] = alphabet[val & 0x3F];
    val >>= 6;
    out[index+0] = alphabet[val & 0x3F];
  }
  out[index] = '\0';
  return out;
}

int b64_decodedlength(char* data) {
  int datalength = strlen(data);
  int len = ((datalength + 3) / 4) * 3;
  if (datalength>0 && data[datalength-1] == '=') --len;
  if (datalength>1 && data[datalength-2] == '=') --len;
  return len;
}

char* b64_decode(char* data, char* out) {
  int datalength = strlen(data);
  int len = ((datalength + 3) / 4) * 3;
  int shift = 0;   // # of excess bits stored in accum
  int accum = 0;   // excess bits
  int index = 0;
  int i, ix;
  char codes[256];
  for (i=0; i<256; i++) codes[i] = -1;
  for (i = 'A'; i <= 'Z'; i++) codes[i] = (char)(     i - 'A');
  for (i = 'a'; i <= 'z'; i++) codes[i] = (char)(26 + i - 'a');
  for (i = '0'; i <= '9'; i++) codes[i] = (char)(52 + i - '0');
  codes['+'] = 62;
  codes['/'] = 63;
  if (datalength>0 && data[datalength-1] == '=') --len;
  if (datalength>1 && data[datalength-2] == '=') --len;
  for (ix=0; ix<datalength; ix++) {
    int value = codes[ data[ix] & 0xFF ];   // ignore high byte of chr
    if ( value >= 0 ) {                     // skip over non-code
      accum <<= 6;            // bits shift up by 6 each time thru
      shift += 6;             // loop, with new bits being put in
      accum |= value;         // at the bottom.
      if ( shift >= 8 ) {     // whenever there are 8 or more shifted in,
        shift -= 8;           // write them out (from the top, leaving any
        out[index++] =        // excess at the bottom for next iteration.
          (char) ((accum >> shift) & 0xff);
      }
    }
  }
  if (index != len) return NULL;
  return out;
}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
}
#endif
