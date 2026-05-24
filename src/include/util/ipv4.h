#ifndef _UTIL_IPV4_H_
#define _UTIL_IPV4_H_

extern
int ipv4_parse
  (const char* string, uint32_t* address);

extern
char* ipv4_tostring
  (uint32_t address);

#endif
