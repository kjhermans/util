#ifndef _UTIL_IO_H_
#define _UTIL_IO_H_

extern
int absorb_file
  (const char* path, unsigned char** buf, unsigned* buflen);

extern
int write_insistent
  (int fd, void* ptr, unsigned size, int lock);

#endif
