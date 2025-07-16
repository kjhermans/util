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

#define DB_KEY_SIZE             128

#define DB_FIELDTYPE_NULL       0
#define DB_FIELDTYPE_BOOLEAN    1
#define DB_FIELDTYPE_INT        2
#define DB_FIELDTYPE_FLOAT      3
#define DB_FIELDTYPE_STRING     4

extern
int table_insert_row
  (td_t* db, const char* table, unsigned nfields, ...);

extern
int table_get_id
  (td_t* db, const char* table, uint64_t* id);

#endif
