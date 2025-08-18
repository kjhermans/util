#include <string.h>
#include <stdio.h>
//#include <util/db_t.h>
#include <util/table_t.h>
#include <fcntl.h>
#include <time.h>

static
char* fakeword
  (char in[ 16 ])
{
  unsigned l = (rand() % 8) + 2;
  for (unsigned i=0; i < l; i++) {
    in[ i ] = 'a' + (rand() % 26);
    in[ i+1 ] = 0;
  }
  return in;
}

int main
  (int argc, char* argv[])
{
  (void)argc;
  (void)argv;

  srand(time(0));

  db_t db = { 0 };

  db_open(&db, "/tmp/db.db", O_RDWR|O_CREAT|O_TRUNC);

  {
    if (table_insert_row(&db, "users", 3,
          "user_name", DB_FIELDTYPE_STRING, "Katherine",
          "user_email", DB_FIELDTYPE_STRING, "foo@bar.com",
          "user_age", DB_FIELDTYPE_INTEGER, 32
    ))
    {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    unsigned l = 0;
    if (table_get_size(&db, "users", &l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    if (l != 1) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    for (unsigned i=0; i < 128; i++) {
      if (table_insert_row(&db, "users", 3,
            "user_name", DB_FIELDTYPE_STRING, "Katherine",
            "user_email", DB_FIELDTYPE_STRING, "foo@bar.com",
            "user_age", DB_FIELDTYPE_INTEGER, 32
      ))
      {
        fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
        db_debug(&db);
        return ~0;
      }
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    unsigned l = 0;
    if (table_get_size(&db, "users", &l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    if (l != 129) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    for (unsigned i=0; i < 127; i++) {
      if (table_delete_row(&db, "users", (uint64_t)(i+1))) {
        fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
        return ~0;
      }
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    unsigned l = 0;
    if (table_get_size(&db, "users", &l)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    if (l != 2) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    char words[ 2 ][ 16 ];
    for (unsigned i=0; i < 128; i++) {
      if (table_insert_row(&db, "users", 3,
            "user_name", DB_FIELDTYPE_STRING, fakeword(words[ 0 ]),
            "user_email", DB_FIELDTYPE_STRING, fakeword(words[ 1 ]),
            "user_age", DB_FIELDTYPE_INTEGER, (rand() % 70)
      ))
      {
        fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
        return ~0;
      }
    }
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    row_t row = { 0 };
    if (table_get_row(&db, "users", 200, &row)) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    for (unsigned i=0; i < row.fields.count; i++) {
      fprintf(stderr, "Row %s : %s\n"
                      , row.fields.list[ i ].name
                      , (char*)row.fields.list[ i ].value.data);
    }
    fprintf(stderr, "Row username: %s\n", row_get_as_string(&row, "user_name"));
  }
  fprintf(stderr, "Subtest Ok.\n");

  fprintf(stderr, "SUCCESS.\n");

  return 0;
}
