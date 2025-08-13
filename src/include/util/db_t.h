#ifndef _DB_H_
#define _DB_H_

#include <unistd.h>
#include <fcntl.h>

#include <util/vec_t.h>

#define DB_KEYBITS      10
#define DB_KEYSIZE      (1<<DB_KEYBITS)
#define DB_KEYMASK      (DB_KEYSIZE-1)

#define DB_SIZEBITS     31
#define DB_SIZEMAX      (1<<DB_SIZEBITS)
#define DB_LOGFACTOR    2
#define DB_IXTUPSIZE    (DB_SIZEBITS>>DB_LOGFACTOR)

#define DB_FLAG_DEL     (1<<0)

#define DB_ERROR        ~0
#define DB_ERR_NOTFOUND 1
#define DB_ERR_EOF      2

typedef struct
{
  int                   fd;
  int                   ix;
}
db_t;

struct db_tuple
{
  unsigned              flags;
  char                  key[ DB_KEYSIZE ];
  unsigned              keysize;
  unsigned              valuesize;
};

struct db_path_node
{
  unsigned              tuple_offset;
  unsigned              level;
  unsigned              next[ DB_IXTUPSIZE ];
};

struct db_path
{
  struct {
    off_t                 offset;
    struct db_path_node   node;
    struct db_tuple       tuple;
  }                     nodes[ DB_IXTUPSIZE ];
  unsigned              length;
  int                   found;
};

struct db_cursor
{
  const db_t*           db;
  off_t                 offset;
  struct db_tuple       tuple;
};

struct db_xcursor
{
  const db_t*           db;
  off_t                 node_offset;
  struct db_path_node   node;
  struct db_tuple       tuple;
};

extern
int db_open
  (db_t* db, const char* path, const int openflags);

extern
void db_close
  (db_t* db);

extern
int db_put
  (const db_t* db, const char* key, const vec_t* value);

extern
int db_get
  (const db_t* db, const char* key, vec_t* value);

extern
int db_del
  (const db_t* db, const char* key);

extern
void db_debug 
  (const db_t* db);

/** Cursor that iterates the database unordered **/

extern
int db_cursor_init 
  (const db_t* db, struct db_cursor* cursor);

extern
int db_cursor_next
  (struct db_cursor* cursor);

extern
int db_cursor_get 
  (struct db_cursor* cursor, char** key, vec_t* value);

/** Cursor that iterates the database in order of keys **/

extern
int db_xcursor_init
  (const db_t* db, struct db_xcursor* cursor);

extern
int db_xcursor_move
  (struct db_xcursor* cursor, char* key, int partial, int exact);

extern
int db_xcursor_next
  (struct db_xcursor* cursor);

extern
int db_xcursor_get
  (struct db_xcursor* cursor, char** key, vec_t* value);

extern
int db_xcursor_first
  (struct db_xcursor* cursor);

#endif
