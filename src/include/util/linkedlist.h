#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdlib.h>
#include <string.h>

#define COMBINE(a, b) a##b

#ifndef LLIST_EQUALS
#define LLIST_EQUALS(a,b) (a == b)
#endif

#define LLIST_BREAK (1<<16) /* Don't define this as an error code */

#define MAKE_LLIST_HEADER(T, prefix)                                \
  typedef struct COMBINE(prefix, strct) COMBINE(prefix, t);         \
  struct COMBINE(prefix, strct) {                                   \
    T value;                                                        \
    COMBINE(prefix, t)* prev;                                       \
    COMBINE(prefix, t)* next;                                       \
  };                                                                \
                                                                    \
  extern                                                            \
  void COMBINE(prefix, init)(COMBINE(prefix, t)* list);             \
                                                                    \
  extern                                                            \
  COMBINE(prefix, t) COMBINE(prefix, INIT)(void);                   \
                                                                    \
  extern                                                            \
  T* COMBINE(prefix, push)(COMBINE(prefix, t)* list, T elt);        \
                                                                    \
  extern                                                            \
  unsigned COMBINE(prefix, size)(COMBINE(prefix, t)* list);         \
                                                                    \
  extern                                                            \
  int COMBINE(prefix, peek)(COMBINE(prefix, t)* list, T* elt);      \
                                                                    \
  extern                                                            \
  T* COMBINE(prefix, peekptr)(COMBINE(prefix, t)* list);            \
                                                                    \
  extern                                                            \
  int COMBINE(prefix, pop)(COMBINE(prefix, t)* list, T* elt);       \
                                                                    \
  extern                                                            \
  int COMBINE(prefix, get)(COMBINE(prefix, t)* list, unsigned index, T* elt);\
                                                                    \
  extern                                                            \
  T* COMBINE(prefix, getptr)(COMBINE(prefix, t)* list, unsigned index);\
                                                                    \
  extern                                                            \
  T* COMBINE(prefix, has)(COMBINE(prefix, t)* list, T elt);         \
                                                                    \
  extern                                                            \
  int COMBINE(prefix, indexof)(COMBINE(prefix, t)* list, T elt);    \
                                                                    \
  extern                                                            \
  int COMBINE(prefix, lastindexof)(COMBINE(prefix, t)* list, T elt);\
                                                                    \
  extern                                                            \
  int COMBINE(prefix, set)(COMBINE(prefix, t)* list, unsigned index, T elt);\
                                                                    \
  extern                                                            \
  int COMBINE(prefix, rem)(COMBINE(prefix, t)* list, unsigned index, T* elt);\
                                                                    \
  extern                                                            \
  int COMBINE(prefix, ins)(COMBINE(prefix, t)* list, unsigned index, T elt);\
                                                                    \
  extern                                                            \
  void COMBINE(prefix, free)(COMBINE(prefix, t)* list);             \
                                                                    \
  extern                                                            \
  void COMBINE(prefix, print)(COMBINE(prefix, t)* list);            \
                                                                    \
  extern                                                            \
  void COMBINE(prefix, copy)(COMBINE(prefix, t)* src,               \
                             COMBINE(prefix, t)* dst);              \
                                                                    \
  extern                                                            \
  int COMBINE(prefix, iterate)(COMBINE(prefix, t)* list,            \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,T*,void*), void*);       \
                                                                    \
  extern                                                            \
  int COMBINE(prefix, reverse)(COMBINE(prefix, t)* list,            \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,T*,void*), void*);       \
                                                                    \

#define MAKE_LLIST_CODE(T, prefix)                                  \
  void COMBINE(prefix, init)(COMBINE(prefix, t)* list) {            \
    memset(list, 0, sizeof(*list));                                 \
  }                                                                 \
                                                                    \
  COMBINE(prefix, t) COMBINE(prefix, INIT)(void) {                  \
    COMBINE(prefix, t) result = { 0 };                              \
    return result;                                                  \
  }                                                                 \
                                                                    \
  unsigned COMBINE(prefix, size)(COMBINE(prefix, t)* list) {        \
    unsigned i=0;                                                   \
    COMBINE(prefix, t)* stepper = list;                             \
    if (NULL == stepper->next) { return 0; }                        \
    while ((NULL != (stepper = stepper->next)) && stepper != list) {\
      ++i;                                                          \
    }                                                               \
    return i+1;                                                     \
  }                                                                 \
                                                                    \
  COMBINE(prefix, t)* COMBINE(prefix, iter)                         \
    (COMBINE(prefix, t)* list, unsigned index)                      \
  {                                                                 \
    COMBINE(prefix, t)* stepper = list;                             \
    for (unsigned i=0; i < index; i++) {                            \
      if (stepper == list) { return NULL; }                         \
      stepper = stepper->next;                                      \
    }                                                               \
    return stepper;                                                 \
  }                                                                 \
                                                                    \
  T* COMBINE(prefix, push)                                          \
    (COMBINE(prefix, t)* list, T elt)                               \
  {                                                                 \
    if (list->next == NULL) {                                       \
      list->next = list;                                            \
      list->prev = list;                                            \
      list->value = elt;                                            \
      return &(list->value);                                        \
    } else {                                                        \
      COMBINE(prefix, t)* node = calloc(sizeof(COMBINE(prefix, t)), 1); \
      COMBINE(prefix, t)* last = list->prev;                        \
      node->next = list;                                            \
      node->prev = last;                                            \
      node->value = elt;                                            \
      last->next = node;                                            \
      list->prev = node;                                            \
      return &(node->value);                                        \
    }                                                               \
  }                                                                 \
                                                                    \
  T* COMBINE(prefix, has)(COMBINE(prefix, t)* list, T elt) {        \
    COMBINE(prefix, t)* node = list;                                \
    unsigned i = 0;                                                 \
    if (node->next) {                                               \
      do {                                                          \
        if (LLIST_EQUALS(node->value, elt)) {                       \
          return &(node->value);                                    \
        }                                                           \
        ++i;                                                        \
      } while ((node = node->next) != list);                        \
    }                                                               \
    return NULL;                                                    \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, get)                                          \
    (COMBINE(prefix, t)* list, unsigned index, T* elt)              \
  {                                                                 \
    COMBINE(prefix, t)* node = COMBINE(prefix, iter)(list, index);  \
    if (NULL == node) { return ~0; }                                \
    if (elt) { *elt = node->value; }                                \
    return 0;                                                       \
  }                                                                 \
                                                                    \
  T* COMBINE(prefix, getptr)                                        \
    (COMBINE(prefix, t)* list, unsigned index)                      \
  {                                                                 \
    COMBINE(prefix, t)* node = COMBINE(prefix, iter)(list, index);  \
    if (NULL == node) { return NULL; }                              \
    return &(node->value);                                          \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, peek)                                         \
    (COMBINE(prefix, t)* list, T* elt)                              \
  {                                                                 \
    if (list->prev) { if (elt) { *elt = list->prev->value; } return 0; } \
    return ~0;                                                      \
  }                                                                 \
                                                                    \
  T* COMBINE(prefix, peekptr)                                       \
    (COMBINE(prefix, t)* list)                                      \
  {                                                                 \
    if (list->prev) { return &(list->prev->value); }                \
    return NULL;                                                    \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, pop)                                          \
    (COMBINE(prefix, t)* list, T* elt)                              \
  {                                                                 \
    if (NULL == list->prev) { return ~0; }                          \
    if (list->prev == list) {                                       \
      if (elt) { *elt = list->value; }                              \
      list->prev = NULL;                                            \
      list->next = NULL;                                            \
      return 0;                                                     \
    } else {                                                        \
      COMBINE(prefix, t)* last = list->prev;                        \
      COMBINE(prefix, t)* lastprev = last->prev;                    \
      if (elt) { *elt = last->value; }                              \
      lastprev->next = last->next;                                  \
      list->prev = lastprev;                                        \
      free(last);                                                   \
      return 0;                                                     \
    }                                                               \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, rem)                                          \
    (COMBINE(prefix, t)* list, unsigned index, T* elt)              \
  {                                                                 \
    COMBINE(prefix, t)* node = COMBINE(prefix, iter)(list, index);  \
    if (NULL == node) { return ~0; }                                \
    if (elt) { *elt = node->value; }                                \
    if (node->next == node->prev && node->prev == list) {           \
      free(node);                                                   \
      list->next = NULL;                                            \
      list->prev = NULL;                                            \
      return 0;                                                     \
    } else {                                                        \
      COMBINE(prefix, t)* prev = node->prev;                        \
      COMBINE(prefix, t)* next = node->next;                        \
      free(node);                                                   \
      prev->next = next;                                            \
      next->prev = prev;                                            \
      return 0;                                                     \
    }                                                               \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, set)(COMBINE(prefix, t)* list, unsigned index, T elt) { \
    COMBINE(prefix, t)* node = COMBINE(prefix, iter)(list, index);  \
    if (NULL == node) { return ~0; }                                \
    node->value = elt;                                              \
    return 0;                                                       \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, ins)                                          \
    (COMBINE(prefix, t)* list, unsigned index, T elt)               \
  {                                                                 \
    COMBINE(prefix, t)* node = COMBINE(prefix, iter)(list, index);  \
    if (NULL == node) { return ~0; }                                \
    COMBINE(prefix, t)* nodenext = node->next;                      \
    COMBINE(prefix, t)* nodenew = calloc(sizeof(COMBINE(prefix, t)), 1); \
    nodenew->next = nodenext;                                       \
    nodenew->prev = node;                                           \
    nodenew->value = elt;                                           \
    node->next = nodenew;                                           \
    nodenext->prev = nodenew;                                       \
    return 0;                                                       \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, iterate)(COMBINE(prefix, t)* list,            \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,T*,void*), void* arg)    \
  {                                                                 \
    COMBINE(prefix, t)* node = list;                                \
    unsigned i = 0;                                                 \
    if (node->next) {                                               \
      do {                                                          \
        int r;                                                      \
        switch (r = fnc(list, i, &(node->value), arg)) {            \
        case 0:                                                     \
          break;                                                    \
        case LLIST_BREAK:                                           \
          return 0;                                                 \
        default:                                                    \
          return r;                                                 \
        }                                                           \
        ++i;                                                        \
      } while ((node = node->next) != list);                        \
    }                                                               \
    return 0;                                                       \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, reverse)(COMBINE(prefix, t)* list,            \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,T*,void*), void* arg)    \
  {                                                                 \
    COMBINE(prefix, t)* node = list;                                \
    unsigned i = 0;                                                 \
    if (node->prev) {                                               \
      while (1) {                                                   \
        int r;                                                      \
        node = node->prev;                                          \
        switch (r = fnc(list, i, &(node->value), arg)) {            \
        case 0:                                                     \
          break;                                                    \
        case LLIST_BREAK:                                           \
          return 0;                                                 \
        default:                                                    \
          return r;                                                 \
        }                                                           \
        ++i;                                                        \
        if (node == list) { break; }                                \
      }                                                             \
    }                                                               \
    return 0;                                                       \
  }                                                                 \
                                                                    \
  int COMBINE(prefix, indexof)(COMBINE(prefix, t)* list, T elt) {   \
    COMBINE(prefix, t)* node = list;                                \
    unsigned i = 0;                                                 \
    if (node->next) {                                               \
      do {                                                          \
        if (LLIST_EQUALS(node->value, elt)) {                       \
          return (int)i;                                            \
        }                                                           \
        ++i;                                                        \
      } while ((node = node->next) != list);                        \
    }                                                               \
    return -1;                                                      \
  }                                                                 \
                                                                    \
  void COMBINE(prefix, free)(COMBINE(prefix, t)* list)              \
  {                                                                 \
    COMBINE(prefix, t)* next = list->next;                          \
    while (next && next != list) {                                  \
      COMBINE(prefix, t)* prev = next;                              \
      next = next->next;                                            \
      free(prev);                                                   \
    }                                                               \
    list->next = NULL;                                              \
    list->prev = NULL;                                              \
  }                                                                 \
                                                                    \

#endif
