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

#include <stdarg.h>

#include <andy/hash.h>

#undef MAP_EQUALS
#define MAP_EQUALS(a,b) (0 == vec_compare(&a, &b))
MAKE_MAP_CODE(vec_t, vec_t, hash_bucket_)

#undef MAP_EQUALS
#define MAP_EQUALS(a,b) (a==b)
MAKE_MAP_CODE(uint8_t, hash_tuple_t*, hash_map_)

extern
uint64_t
chibihash64
  (const void *keyIn, long len, uint64_t seed)
  __attribute__ ((warn_unused_result));

static
uint64_t hash_default_hasher
  (hash_t* h, vec_t* key, void* arg)
{
  (void)h;
  (void)arg;
  return chibihash64(key->data, key->size, 0);
}

/**
 *
 */
void hash_init
  (hash_t* h)
{
  memset(h, 0, sizeof(*h));
  h->hasher = hash_default_hasher;
  h->depth = 8; // max is 16 (* 4 == 64)
  h->replacepolicy = HASH_REPLACE_ERROR;
}

static
void hash_free_bucket
  (hash_t* h, hash_bucket_t* bucket)
{
  for (unsigned i=0; i < bucket->count; i++) {
    vec_t* key = &(bucket->keys[ i ]);
    vec_t* value = &(bucket->values[ i ]);
    switch (h->ownership.policy) {
    case HASH_OWNER_CALLER:
      break;
    case HASH_OWNER_LIBRARY:
      free(key->data);
      free(value->data);
      break;
    case HASH_OWNER_CALLBACK:
      h->ownership.callback(h, key, value, h->ownership.arg);
      break;
    }
  }
}

static
void hash_free_map
  (hash_t* h, hash_map_t* map, unsigned d)
{
  for (unsigned i=0; i < map->count; i++) {
    hash_tuple_t* tuple = map->values[ i ];
    if (d < h->depth) {
      hash_free_map(h, &(tuple->map), d+1);
    } else {
      hash_free_bucket(h, &(tuple->bucket));
    }
    free(tuple);
  }
  free(map->keys);
  free(map->values);
}

void hash_free
  (hash_t* h)
{
  hash_map_t* map = &(h->map);
  hash_free_map(h, map, 0);
}

void hash_set_hasher
  (hash_t* h, uint64_t(*fnc)(hash_t* h,vec_t* key, void* arg), void* arg)
{
  h->hasher = fnc;
  h->arg = arg;
}

void hash_set_depth
  (hash_t* h, unsigned nbits)
{
  h->depth = nbits;
}

void hash_set_replace_policy
  (hash_t* h, unsigned pol)
{
  h->replacepolicy = pol;
}

void hash_set_ownership_policy
  (hash_t* h, unsigned pol, ...)
{
  h->ownership.policy = pol;
  if (pol == HASH_OWNER_CALLBACK) {
    va_list ap = { 0 };
    va_start(ap, pol);
    h->ownership.callback = va_arg(ap, hash_own_free_t);
    h->ownership.arg = va_arg(ap, void*);
  }
}

int hash_put
  (hash_t* h, vec_t* key, vec_t* value)
{
  hash_map_t* map = &(h->map);
  uint64_t hash = h->hasher(h, key, h->arg);
  hash_tuple_t* tuple = NULL;
  vec_t* found = NULL;

  hash %= ((uint64_t)1 << (uint64_t)(h->depth * 4));
  for (unsigned i=0; i < h->depth; i++) {
    uint8_t nibble = (hash & 0x0f);
    hash >>= 4;
    if (hash_map_get(map, nibble, &tuple)) {
      tuple = calloc(1, sizeof(hash_tuple_t));
      hash_map_put(map, nibble, tuple);
    }
    map = &(tuple->map);
  }
  if ((found = hash_bucket_getptr(&(tuple->bucket), *key)) != NULL) {
    switch (h->replacepolicy) {
    case HASH_REPLACE_REPLACE:
      switch (h->ownership.policy) {
      case HASH_OWNER_CALLER:
        break;
      case HASH_OWNER_LIBRARY:
        free(found->data);
        break;
      case HASH_OWNER_CALLBACK:
        h->ownership.callback(h, NULL, found, h->ownership.arg);
        break;
      }
      found->data = value->data;
      found->size = value->size;
      break;
    case HASH_REPLACE_REJECT:
      break;
    case HASH_REPLACE_ERROR:
      return HASH_ERR_REPLACE;
    }
  } else {
    hash_bucket_put(&(tuple->bucket), *key, *value);
  }
  return 0;
}

int hash_get
  (hash_t* h, vec_t* key, vec_t* value)
{
  hash_map_t* map = &(h->map);
  uint64_t hash = h->hasher(h, key, h->arg);
  hash_tuple_t* tuple;

  hash %= ((uint64_t)1 << (uint64_t)(h->depth * 4));
  for (unsigned i=0; i < h->depth; i++) {
    uint8_t nibble = (hash & 0x0f);
    hash >>= 4;
    if (hash_map_get(map, nibble, &tuple)) {
      return HASH_ERR_NOTFOUND;
    }
    map = &(tuple->map);
  }
  if (hash_bucket_get(&(tuple->bucket), *key, value)) {
    return HASH_ERR_NOTFOUND;
  } else {
    return 0;
  }
}

int hash_del
  (hash_t* h, vec_t* key, vec_t* value)
{
  hash_map_t* map = &(h->map);
  uint64_t hash = h->hasher(h, key, h->arg);
  hash_tuple_t* tuple;

  hash %= ((uint64_t)1 << (uint64_t)(h->depth * 4));
  for (unsigned i=0; i < h->depth; i++) {
    uint8_t nibble = (hash & 0x0f);
    hash >>= 4;
    if (hash_map_get(map, nibble, &tuple)) {
      return HASH_ERR_NOTFOUND;
    }
    map = &(tuple->map);
  }
  if (hash_bucket_del(&(tuple->bucket), *key, value)) {
    return HASH_ERR_NOTFOUND;
  } else {
    return 0;
  }
}
