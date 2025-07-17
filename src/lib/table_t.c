#include <inttypes.h>

#include <util/table_t.h>

#undef ARRAY_EQUALS
#define ARRAY_EQUALS(a,b) (&a == &b)
MAKE_ARRAY_CODE(vec_t, row_)
MAKE_ARRAY_CODE(row_t, table_)

static
int __table_get_size
  (td_t* db, const char* table, unsigned* nrows)
{
  char keystr[ DB_KEY_SIZE ];
  tdt_t cache = { keystr, 0 };
  tdt_t value = { nrows, sizeof(unsigned) };

  snprintf(keystr, sizeof(keystr), "SIZ_%s", table);
  cache.size = strlen(keystr);
  return td_get(db, &cache, &value, TDFLG_EXACT);
}

static
int __table_set_size
  (td_t* db, const char* table, unsigned nrows)
{
  char keystr[ DB_KEY_SIZE ];
  tdt_t cache = { keystr, 0 };
  tdt_t value = { &nrows, sizeof(unsigned) };

  snprintf(keystr, sizeof(keystr), "SIZ_%s", table);
  cache.size = strlen(keystr);
  return td_put(db, &cache, &value, 0);
}

/**
 * Example:

table_insert_row(
  &db, "Users", 3,
  "Firstname", DB_TYPE_STRING, "Katherine",
  "Emailaddress", DB_TYPE_NULL,                // Note the lack of 3rd param
  "Age", DB_TYPE_INTEGER, 32
);

 */
int table_insert_row
  (td_t* db, const char* table, unsigned nfields, ...)
{
  uint64_t id = 0;
  unsigned i = 0;
  va_list ap = { 0 };

  if (table_get_id(db, table, &id)) {
    return ~0;
  }
  va_start(ap, nfields);
  for (i=0; i < nfields; i++) {
    const char* fieldname = va_arg(ap, const char*);
    unsigned fieldtype = va_arg(ap, unsigned);
    char valuebuffer[ 64 ];
    char* fieldvalue_str = valuebuffer;
    int64_t fieldvalue_int = 0;
    double fieldvalue_float = 0;
    char keystr[ DB_KEY_SIZE ];
    tdt_t key = { keystr, 0 }, val = { fieldvalue_str, 0 };

    switch (fieldtype) {
    case DB_FIELDTYPE_NULL:
      snprintf(valuebuffer, sizeof(valuebuffer), "null");
      break;
    case DB_FIELDTYPE_BOOLEAN:
      fieldvalue_int = va_arg(ap, int64_t);
      if (fieldvalue_int) {
        snprintf(valuebuffer, sizeof(valuebuffer), "true");
      } else {
        snprintf(valuebuffer, sizeof(valuebuffer), "false");
      }
      break;
    case DB_FIELDTYPE_INTEGER:
      fieldvalue_int = va_arg(ap, int64_t);
      snprintf(valuebuffer, sizeof(valuebuffer), "%"PRId64, fieldvalue_int);
      break;
    case DB_FIELDTYPE_FLOAT:
      fieldvalue_float = va_arg(ap, double);
      snprintf(valuebuffer, sizeof(valuebuffer), "%f", fieldvalue_float);
      break;
    case DB_FIELDTYPE_STRING:
      fieldvalue_str = va_arg(ap, char*);
      break;
    }
    snprintf(keystr, sizeof(keystr),
             "TUP_%s_%.20"PRIu64"_%s", table, id, fieldname);
    key.size = strlen(keystr);
    val.size = strlen(fieldvalue_str);
    if (td_put(db, &key, &val, 0)) {
      return ~0;
    }
  }
  va_end(ap);

  {
    unsigned nrows = 0;
    if (__table_get_size(db, table, &nrows) == 0) {
      ++nrows;
      return __table_set_size(db, table, nrows);
    } else {
      return __table_set_size(db, table, (unsigned)1);
    }
  }

  return 0;
}

int table_get_size
  (td_t* db, const char* table, unsigned* nrows)
{
  tdc_t cursor;
  char searchkeystr[ DB_KEY_SIZE ];
  tdt_t search = { searchkeystr, 0 };
  uint64_t id = 0;

  if (__table_get_size(db, table, nrows) == 0) {
    return 0;
  }

  snprintf(searchkeystr, sizeof(searchkeystr), "TUP_%s_", table);
  search.size = strlen(searchkeystr);
  tdc_init(db, &cursor);
  if (tdc_mov(&cursor, &search, TDFLG_PARTIAL|TDFLG_EXACT)) {
    return ~0;
  }
  *nrows = 0;
  while (1) {
    char keystr[ DB_KEY_SIZE ];
    tdt_t key = { keystr, sizeof(keystr) };
    if (tdc_get(&cursor, &key, 0, TDFLG_ALLOCTDT) == 0) {
      char* numstr = key.data + search.size;
      uint64_t foundid;
      numstr[ 20 ] = 0;
      foundid = strtoull(numstr, 0, 10);
      if (foundid != id) {
        id = foundid;
        ++(*nrows);
      }
    }
    if (tdc_nxt(&cursor, 0, 0, 0)) {
      break;
    }
  }
  return __table_set_size(db, table, *nrows);
}

int table_get_id
  (td_t* db, const char* table, uint64_t* id)
{
  char keystr[ DB_KEY_SIZE ] = { 0 };
  tdt_t key = { keystr, 0 }, val = { id, sizeof(*id) };

  snprintf(keystr, sizeof(keystr), "SEQ_%s", table);
  key.size = strlen(keystr);
  if (td_get(db, &key, &val, TDFLG_EXACT)) {
    *id = 1;
    return td_put(db, &key, &val, 0);
  } else {
    ++(*id);
    return td_put(db, &key, &val, 0);
  }
}

