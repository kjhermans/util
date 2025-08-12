#include <string.h>
#include <stdio.h>
#include <time.h>

#include <util/db_t.h>

void db_walk
  (db_t* db)
{
  struct db_xcursor c = { 0 };

  db_xcursor_init(db, &c);
  while (1) {
    char* key = 0;
    vec_t value = { 0 }; 
    if (db_xcursor_get(&c, &key, &value)) { break; }
    fprintf(stderr, "Key '%s'\n", key);
    if (db_xcursor_next(&c)) { break; }
  }
}

int main
  (int argc, char* argv[])
{
  (void)argc;
  (void)argv;
srand(time(0));

  {
    db_t db = { 0 };
    int r = db_open(&db, "/tmp/db.db", O_RDWR|O_CREAT|O_TRUNC);
    if (r) {
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
    db_close(&db);
    unlink("/tmp/db.db");
    unlink("/tmp/db.db.index");
  }
  fprintf(stderr, "Subtest Ok.\n");

  {
    db_t db = { 0 };
    char* key = "foo";
    vec_t value = { "bar", 3 };
    int r = db_open(&db, "/tmp/db.db", O_RDWR|O_CREAT|O_TRUNC);
    if (r) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    if ((r = db_put(&db, key, &value)) != 0) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    db_debug(&db);
    db_close(&db);
    unlink("/tmp/db.db");
    unlink("/tmp/db.db.index");
  }
  fprintf(stderr, "Subtest Ok.\n");

  { // second put has key > first key, so natural order.
    db_t db = { 0 };
    char* key0 = "foo";
    char* key1 = "oi";
    vec_t value = { "bar", 3 };
    int r = db_open(&db, "/tmp/db.db", O_RDWR|O_CREAT|O_TRUNC);
    if (r) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    if ((r = db_put(&db, key0, &value)) != 0) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    if ((r = db_put(&db, key1, &value)) != 0) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    db_debug(&db);
    db_close(&db);
    unlink("/tmp/db.db");
    unlink("/tmp/db.db.index");
  }
  fprintf(stderr, "Subtest Ok.\n");

  { // second put has key < first key, so should be ordered before.
    db_t db = { 0 };
    char* key0 = "foo";
    char* key1 = "aaa";
    vec_t value = { "bar", 3 };
    int r = db_open(&db, "/tmp/db.db", O_RDWR|O_CREAT|O_TRUNC);
    if (r) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    if ((r = db_put(&db, key0, &value)) != 0) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    if ((r = db_put(&db, key1, &value)) != 0) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    db_debug(&db);
    db_walk(&db);
    db_close(&db);
    unlink("/tmp/db.db");
    unlink("/tmp/db.db.index");
  }
  fprintf(stderr, "Subtest Ok.\n");

  { // 26 keys in order.
    db_t db = { 0 };
    char key[ 2 ] = { 0 };
    vec_t value = { "bar", 3 };
    int r = db_open(&db, "/tmp/db.db", O_RDWR|O_CREAT|O_TRUNC);
    if (r) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    for (unsigned i='a'; i <= 'z'; i++) {
      key[ 0 ] = i;
      if ((r = db_put(&db, key, &value)) != 0) {
        fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
        return ~0;
      }
      db_debug(&db);
    }
    db_debug(&db);
    db_walk(&db);
    db_close(&db);
    unlink("/tmp/db.db");
    unlink("/tmp/db.db.index");
  }
  fprintf(stderr, "Subtest Ok.\n");

  { // 26 keys in random order.
    char keys[] = "plokimjunhybgtvfrcdexswzaq";
    db_t db = { 0 };
    char key[ 2 ] = { 0 };
    vec_t value = { "bar", 3 };
    int r = db_open(&db, "/tmp/db.db", O_RDWR|O_CREAT|O_TRUNC);
    if (r) {
      fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
      return ~0;
    }
    for (unsigned i=0; i < sizeof(keys)-1; i++) {
      key[ 0 ] = keys[ i ];
      if ((r = db_put(&db, key, &value)) != 0) {
        fprintf(stderr, "FAILURE (%d at %s:%d).\n", r, __FILE__, __LINE__);
        return ~0;
      }
      db_debug(&db);
    }
    db_debug(&db);
    db_walk(&db);
    db_close(&db);
    unlink("/tmp/db.db");
    unlink("/tmp/db.db.index");
  }
  fprintf(stderr, "Subtest Ok.\n");

/*
  {
    if (
      fprintf(stderr, "FAILURE (%s:%d).\n", __FILE__, __LINE__);
      return ~0;
    }
  }
  fprintf(stderr, "Subtest Ok.\n");
*/

  fprintf(stderr, "SUCCESS\n");

  return 0;
}
