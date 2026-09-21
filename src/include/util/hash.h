/**
 * This file is part of Andy, my utilities.

Copyright (c) 2026, Kees-Jan Hermans <kees.jan.hermans@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL the copyright holder BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *
 * \file
 * \brief
 */

#ifndef _ANDY_HASH_H_
#define _ANDY_HASH_H_

#include <stdint.h>
#include <andy/map.h>
#include <andy/vec_t.h>

#define HASH_ERR_NOTFOUND       1
#define HASH_ERR_REPLACE        2

MAKE_MAP_HEADER(vec_t, vec_t, hash_bucket_)

typedef union hash_tuple hash_tuple_t;

MAKE_MAP_HEADER(uint8_t, hash_tuple_t*, hash_map_)

union hash_tuple
{
  hash_map_t            map;
  hash_bucket_t         bucket;
};

typedef struct hash hash_t;

typedef void(*hash_own_free_t)(hash_t*,vec_t*,vec_t*,void*);

struct hash
{
  uint64_t           (* hasher)(hash_t* h,vec_t* key, void* arg);
  void*                 arg;
  hash_map_t            map;
  unsigned              depth;
  unsigned              replacepolicy;
  struct {
    unsigned              policy;
    hash_own_free_t       callback;
    void*                 arg;
  }                     ownership;
};

extern
void hash_init
  (hash_t* h);

extern
void hash_set_hasher
  (hash_t* h, uint64_t(*fnc)(hash_t* h,vec_t* key,void* arg), void* arg);

  /** Sets the max depth of the hash. Must be <= 16 **/
extern
void hash_set_depth
  (hash_t* h, unsigned nbits);

#define HASH_REPLACE_REPLACE    1
#define HASH_REPLACE_REJECT     2
#define HASH_REPLACE_ERROR      3
extern
void hash_set_replace_policy
  (hash_t* h, unsigned pol);

  /**
   * When specifying HASH_OWNER_CALLBACK in h->policy pol,
   * the subsequent argument must be a function pointer of type
   * hash_own_free_t, and the subsequent argument a void* (argument).
   *
   * The default is HASH_OWNER_CALLER.
   */
#define HASH_OWNER_CALLER       1
#define HASH_OWNER_LIBRARY      2
#define HASH_OWNER_CALLBACK     3
extern
void hash_set_ownership_policy
  (hash_t* h, unsigned pol, ...);

extern
int hash_put
  (hash_t* h, vec_t* key, vec_t* value)
  __attribute__ ((warn_unused_result));

extern 
int hash_get
  (hash_t* h, vec_t* key, vec_t* value)
  __attribute__ ((warn_unused_result));

extern
int hash_del
  (hash_t* h, vec_t* key, vec_t* value)
  __attribute__ ((warn_unused_result));

#endif
