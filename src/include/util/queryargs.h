#ifndef _QUERYARGS_H_
#define _QUERYARGS_H_

extern
int queryargs
  (
    unsigned argc,
    char* argv[],
    char option,
    char* longoption,
    unsigned index,
    int expectvalue,
    char** remainder,
    char** value
  );

extern
void queryargs_
  (
    unsigned argc,
    char* argv[],
    char option,
    char* longoption,
    unsigned index,
    int expectvalue,
    char** remainder,
    char** value
  );

extern
int queryargs_as_int
  (
    unsigned argc,
    char* argv[],
    char option,
    char* longoption,
    unsigned index,
    char** remainder,
    char** value
  );

extern
char* nextarg
  (unsigned argc, char* argv[], char* arg);

#endif
