#ifndef _TABLE_T_H_
#define _TABLE_T_H_

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include <util/vec_t.h>
#include <util/array.h>
#include <btree/td.h>

MAKE_ARRAY_HEADER(vec_t, row_)
MAKE_ARRAY_HEADER(row_t, table_)

#define DB_KEY_SIZE             256

typedef struct
{
  table_t       table;
  uint64_t      rowid;
  unsigned      offset;
  unsigned      length;
}
cache_t;

#define DB_FIELDTYPE_NULL       0
#define DB_FIELDTYPE_BOOLEAN    1
#define DB_FIELDTYPE_INTEGER    2
#define DB_FIELDTYPE_FLOAT      3
#define DB_FIELDTYPE_STRING     4

extern
void row_deep_free
  (row_t* row);

extern
void table_deep_free
  (table_t* table);

extern
int table_insert_row
  (td_t* db, const char* table, unsigned nfields, ...);

extern
int table_update_row
  (td_t* db, const char* table, uint64_t rowid, unsigned nfields, ...);

extern
int table_delete_row
  (td_t* db, const char* table, uint64_t rowid);

extern
int table_iterate_rows
  (td_t* db, const char* table, int(*fnc)(uint64_t,row_t*,void*), void* arg);

#endif
