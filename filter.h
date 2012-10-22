
#ifndef _FILTER_H
#define _FILTER_H

/* Copyright (c) 2012, Heng.Wang. All rights reserved.

This program is aimed to filter the sensitive value of given field, if
you have more effective processing methods or some ideas to solve
the problem, thanks for your sharing with the developers. It's pleasure
for you to contact me.
@Author: Heng.Wang
@Email: heng.wang@qunar.com
              heng.wang@outlook.com
              wangheng.king@gmail.com
              king_wangheng@163.com
@Github: https://github.com/HengWang/
*/

#include <my_global.h>
#include <my_sys.h>

/* The const variables*/
#ifndef DELIM_KEY
#define DELIM_KEY ","
#endif
#ifndef LINK_KEY
#define LINK_KEY '.'
#endif
#ifndef EQUAL_KEY
#define EQUAL_KEY "="
#define EQUAL_KEY_LEN 1
#endif
#ifndef BIGGER_KEY
#define BIGGER_KEY ">"
#define BIGGER_KEY_LEN 1
#endif
#ifndef BIGGER_EQUAL_KEY
#define BIGGER_EQUAL_KEY ">="
#define BIGGER_EQUAL_KEY_LEN 2
#endif
#ifndef LESS_KEY
#define LESS_KEY "<"
#define LESS_KEY_LEN 1
#endif
#ifndef LESS_EQUAL_KEY
#define LESS_EQUAL_KEY ">"
#define LESS_EQUAL_KEY_LEN 1
#endif
#ifndef LIKE_KEY
#define LIKE_KEY "LIKE"
#define LIKE_KEY_LEN 4
#endif
#ifndef IN_KEY
#define IN_KEY "IN"
#define IN_KEY_LEN 2
#endif
#ifndef FILTER_TYPE
#define FILTER_TYPE 0
#endif
#ifndef WHERE_TYPE
#define WHERE_TYPE 1
#endif

/*
 * Tail queue functions.
 */
#ifndef TAILQ_ENTRY
#define TAILQ_ENTRY(type)						\
struct {								\
struct type *tqe_next;	/* next element */			\
struct type **tqe_prev;	/* address of previous next element */	\
}
#endif /* !TAILQ_ENTRY */

#ifndef TAILQ_HEAD
#define TAILQ_HEAD(name, type)			\
struct name {					\
struct type *tqh_first;		/* first element */		\
struct type **tqh_last;		/* addr of last next element */	\
}
#endif /* !TAIL_HEAD */

#ifndef TAILQ_FIRST
#define	TAILQ_FIRST(head)		((head)->tqh_first)
#endif /* !TAILQ_FIRST */

#ifndef TAILQ_END
#define	TAILQ_END(head)			NULL
#endif /* !TAILQ_END */

#ifndef TAILQ_NEXT
#define	TAILQ_NEXT(elm, field)		((elm)->field.tqe_next)
#endif  /* !TAILQ_NEXT */

#ifndef TAILQ_LAST
#define TAILQ_LAST(head, headname)					\
  (*(((struct headname *)((head)->tqh_last))->tqh_last))
#endif  /* !TAILQ_LAST */

#ifndef TAILQ_PREV
#define TAILQ_PREV(elm, headname, field)				\
  (*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))
#endif /* !TAILQ_PREV */

#ifndef TAILQ_EMPTY
#define	TAILQ_EMPTY(head)						\
  (TAILQ_FIRST(head) == TAILQ_END(head))
#endif  /* !TAILQ_EMPTY */

#ifndef TAILQ_FOREACH
#define TAILQ_FOREACH(var, head, field)					\
  for ((var) = TAILQ_FIRST(head);					\
  (var) != TAILQ_END(head);					\
  (var) = TAILQ_NEXT(var, field))
#endif /* !TAILQ_FOREACH */

#ifndef TAILQ_FOREACH_REVERSE
#define TAILQ_FOREACH_REVERSE(var, head, headname, field)		\
  for((var) = TAILQ_LAST(head, headname);				\
  (var) != TAILQ_END(head);					\
  (var) = TAILQ_PREV(var, headname, field))
#endif  /* !TAILQ_FOREACH_REVERSE */

#ifndef TAILQ_INIT
#define	TAILQ_INIT(head) do {						\
  (head)->tqh_first = NULL;					\
  (head)->tqh_last = &(head)->tqh_first;				\
} while (0)
#endif  /* !TAILQ_INIT */

#ifndef TAILQ_INSERT_HEAD
#define TAILQ_INSERT_HEAD(head, elm, field) do {			\
  if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)	\
  (head)->tqh_first->field.tqe_prev =			\
  &(elm)->field.tqe_next;				\
  else								\
  (head)->tqh_last = &(elm)->field.tqe_next;		\
  (head)->tqh_first = (elm);					\
  (elm)->field.tqe_prev = &(head)->tqh_first;			\
} while (0)
#endif  /* !TAILQ_INSERT_HEAD */

#ifndef TAILQ_INSERT_TAIL
#define TAILQ_INSERT_TAIL(head, elm, field) do {			\
  (elm)->field.tqe_next = NULL;					\
  (elm)->field.tqe_prev = (head)->tqh_last;			\
  *(head)->tqh_last = (elm);					\
  (head)->tqh_last = &(elm)->field.tqe_next;			\
} while (0)
#endif /* !TAILQ_INSERT_TAIL */

#ifndef TAILQ_INSERT_AFTER
#define TAILQ_INSERT_AFTER(head, listelm, elm, field) do {		\
  if (((elm)->field.tqe_next = (listelm)->field.tqe_next) != NULL)\
  (elm)->field.tqe_next->field.tqe_prev =			\
  &(elm)->field.tqe_next;				\
  else								\
  (head)->tqh_last = &(elm)->field.tqe_next;		\
  (listelm)->field.tqe_next = (elm);				\
  (elm)->field.tqe_prev = &(listelm)->field.tqe_next;		\
} while (0)
#endif  /* !TAILQ_INSERT_AFTER */

#ifndef TAILQ_INSERT_BEFORE
#define	TAILQ_INSERT_BEFORE(listelm, elm, field) do {			\
  (elm)->field.tqe_prev = (listelm)->field.tqe_prev;		\
  (elm)->field.tqe_next = (listelm);				\
  *(listelm)->field.tqe_prev = (elm);				\
  (listelm)->field.tqe_prev = &(elm)->field.tqe_next;		\
} while (0)
#endif /* !TAILQ_INSERT_BEFORE */

#ifndef TAILQ_REMOVE
#define TAILQ_REMOVE(head, elm, field) do {				\
  if (((elm)->field.tqe_next) != NULL)				\
  (elm)->field.tqe_next->field.tqe_prev =			\
  (elm)->field.tqe_prev;				\
  else								\
  (head)->tqh_last = (elm)->field.tqe_prev;		\
  *(elm)->field.tqe_prev = (elm)->field.tqe_next;			\
} while (0)
#endif /* !TAILQ_REMOVE */

#ifndef TAILQ_REPLACE
#define TAILQ_REPLACE(head, elm, elm2, field) do {			\
  if (((elm2)->field.tqe_next = (elm)->field.tqe_next) != NULL)	\
  (elm2)->field.tqe_next->field.tqe_prev =		\
  &(elm2)->field.tqe_next;				\
  else								\
  (head)->tqh_last = &(elm2)->field.tqe_next;		\
  (elm2)->field.tqe_prev = (elm)->field.tqe_prev;			\
  *(elm2)->field.tqe_prev = (elm2);				\
} while (0)
#endif /* !TAILQ_REPLACE */

/*
 *  The structure of key value for storing the value of field. 
 */
struct st_key_value{
  char* key;    //The field name
  char* link; //The link symbol.
  char* value;    //The value of replacement on field fd_name. 
  TAILQ_ENTRY(st_key_value) next;
};
//TAILQ_HEAD (st_key_value_head, st_key_value);
struct st_key_value_head {					
  struct st_key_value *tqh_first;		/* first element */		
  struct st_key_value **tqh_last;		/* addr of last next element */	
  uint type;
};
typedef struct st_key_value sensitive_field;
typedef struct st_key_value_head sensitive_field_head;

/*
 *  The structure of table for storing the field list of tb_name. 
 */
struct st_sensitive_table_fields{
  char* tb_name;    //The table name
  sensitive_field_head* field_lists;  //Field lists for replacing into insensitive words.
  sensitive_field_head* where_lists; // Where condition lists to filter dump data.
  uint num_filter; //The number of filter condition.
  uint num_where; //The number of where condition.
  TAILQ_ENTRY(st_sensitive_table_fields) next;
};
TAILQ_HEAD (st_sensitive_table_fields_head, st_sensitive_table_fields);

typedef struct st_sensitive_table_fields sensitive_table;
typedef struct st_sensitive_table_fields_head sensitive_table_head;

/*
 *  The structure of database for storing the table list of db_name. 
 */
struct st_sensitive_db_tables_fields{
  char* db_name;   //The database name 
  sensitive_table_head* table_lists;  //Field lists for replacing into insensitive words.
  TAILQ_ENTRY(st_sensitive_db_tables_fields) next;
};
TAILQ_HEAD (st_sensitive_db_tables_fields_head, st_sensitive_db_tables_fields);

typedef struct st_sensitive_db_tables_fields sensitive_db;
typedef struct st_sensitive_db_tables_fields_head sensitive_db_head;

/*
 * The functions to process the field, table, database.
 */

/*
  Get the value of the given key in the sensitive field lists. 

  SYNOPSIS
    get_value()
    s_fields       The list of sensitive fields.
    key              The given key.

  RETURN VALUES
    NULL               no matched key in the list of sensitive fields;
    value                 the value of the given key.
*/
char* get_value(const sensitive_field_head* s_fields, const char* key);

/*
  Get the field of the given key in the sensitive field lists. 

  SYNOPSIS
    get_field()
    s_fields       The list of sensitive fields.
    s_field         The field that the key matched the given key.
    key              The given key.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int get_field(const sensitive_field_head* s_fields, sensitive_field** s_field, const char* key);

/*
Check the key whether in the sensitive field list or not. 

SYNOPSIS
is_field_exists()
s_fields       The list of sensitive fields.
key               The given key.
link               The given link symbol.
value            The given value.

RETURN VALUES
TRUE          success;
FALSE         failed.
*/
my_bool is_field_exists(const sensitive_field_head* s_fields, const char* key, const char* link, const char* value);

/*
  Initialize the sensitive field head. 

  SYNOPSIS
    init_field_head()
    s_fields         The object of sensitive field head.
    type               The field type.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_field_head(sensitive_field_head** s_fields, const uint type);

/*
  Uninitialize the sensitive field head. 

  SYNOPSIS
    uninit_field_head()
    s_fields         The object of sensitive field head.
*/
void uninit_field_head(sensitive_field_head* s_fields);

/*
  Initialize the sensitive field. 

  SYNOPSIS
    init_field()
    s_field         The object of sensitive field.
    key              The given key.
    link              The link symbol.
    value            The value of the key.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_field(sensitive_field** s_field, const char* key, const char* link, const char* value);

/*
  Uninitialize the sensitive field. 

  SYNOPSIS
    uninit_field()
    s_field         The object of sensitive field.
*/
void uninit_field(sensitive_field* s_field);

/*
  Add the sensitive field into the list. 

  SYNOPSIS
    add_field()
    s_fields       The list of sensitive fields.
    s_field         The object of sensitive field.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int add_field(sensitive_field_head* s_fields, sensitive_field* s_field);

/*
  Remove the sensitive field from the list. 

  SYNOPSIS
    remove_field()
    s_fields       The list of sensitive fields.
    s_field         The object of sensitive field.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int remove_field(sensitive_field_head* s_fields, sensitive_field* s_field);

/*
  Clear all of the sensitive field from the list. 

  SYNOPSIS
    clear_fields()
    s_fields       The list of sensitive fields.
*/
void clear_fields(sensitive_field_head* s_fields);

/*
  Get the sensitive table of given table name from the list. 

  SYNOPSIS
    get_table()
    s_tables       The list of sensitive tables.
    s_table         The object of sensitive table.
    tb                 The table name
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int get_table(const sensitive_table_head* s_tables, sensitive_table** s_table, const char* tb);

/*
  Check the given table name whether in the sensitive table list. 

  SYNOPSIS
    is_table_exists()
    s_tables       The list of sensitive tables.
    tb                 The table name
 
  RETURN VALUES
    TRUE          success;
    FALSE         failed.
*/
my_bool is_table_exists(const sensitive_table_head* s_tables, const char* tb);

/*
  Initialize the sensitive table head. 

  SYNOPSIS
    init_field_head()
    s_tables         The object of sensitive table head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_table_head(sensitive_table_head** s_tables);

/*
  Uninitialize the sensitive table head. 

  SYNOPSIS
    uninit_field_head()
    s_tables         The object of sensitive table head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
void uninit_table_head(sensitive_table_head* s_tables);

#define init_table_filter(A,B,C) init_table(A,B,C,NULL)
#define init_table_where(A,B,C) init_table(A,B,NULL,C)
/*
  Initialize the sensitive table based on given table name. 

  SYNOPSIS
    init_table()
    s_table         The object of sensitive table.
    tb                  The table name
    f_s_fields     The sensitive filter field lists.
    w_s_fields   The sensitive where field lists.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_table(sensitive_table** s_table, const char* tb, sensitive_field_head* f_s_fields, sensitive_field_head* w_s_fields);

/*
  Uninitialize the sensitive table . 

  SYNOPSIS
    uninit_table()
    s_table         The object of sensitive table.
*/
void uninit_table(sensitive_table* s_table);

/*
  Add the sensitive table into the list. 

  SYNOPSIS
    add_table()
    s_tables       The list of sensitive tables.
    s_table         The object of sensitive table.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int add_table(sensitive_table_head* s_tables, sensitive_table* s_table);

/*
  Remove the sensitive table from the list. 

  SYNOPSIS
    remove_table()
    s_tables       The list of sensitive tables.
    s_table         The object of sensitive table.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int remove_table(sensitive_table_head* s_tables, sensitive_table* s_table);

/*
  Clear all of the sensitive table from the list. 

  SYNOPSIS
    clear_tables()
    s_tables       The list of sensitive tables.
*/
void clear_tables(sensitive_table_head* s_tables);

/*
  Get the sensitive database of given database name from the list. 

  SYNOPSIS
    get_database()
    s_dbs            The list of sensitive databases.
    s_db              The object of sensitive database.
    db                  The table name
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int get_database(const sensitive_db_head* s_dbs, sensitive_db** s_db, const char* db);

/*
  Check the sensitive database of given database name  whether in the list or not. 

  SYNOPSIS
    is_db_exists()
    s_dbs            The list of sensitive databases.
    db                  The table name
 
  RETURN VALUES
    TRUE            success;
    FALSE           failed.
*/
my_bool is_db_exists(const sensitive_db_head* s_dbs, const char* db);

/*
  Initialize the sensitive database head. 

  SYNOPSIS
    init_db_head()
    s_dbs         The object of sensitive database head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_db_head(sensitive_db_head** s_dbs);

/*
  Uninitialize the sensitive database head. 

  SYNOPSIS
    uninit_db_head()
    s_dbs         The object of sensitive database head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
void uninit_db_head(sensitive_db_head* s_dbs);

/*
  Initialize the sensitive database of given database name. 

  SYNOPSIS
    init_db()
    s_db              The object of sensitive database.
    db                  The table name
   s_tables         The sensitive table lists.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_db(sensitive_db** s_db, const char* db, sensitive_table_head* s_tables);

/*
  Uninitialize the sensitive database. 

  SYNOPSIS
    uninit_db()
    s_db              The object of sensitive database.
*/
void uninit_db(sensitive_db* s_db);

/*
  Add the sensitive database into the list. 

  SYNOPSIS
    add_db()
    s_dbs            The list of sensitive databases.
    s_db              The object of sensitive database.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int add_db(sensitive_db_head* s_dbs, sensitive_db* s_db);

/*
  Remove the sensitive database from the list. 

  SYNOPSIS
    remove_db()
    s_dbs            The list of sensitive databases.
    s_db              The object of sensitive database.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int remove_db(sensitive_db_head* s_dbs, sensitive_db* s_db);

/*
  Clear all of the sensitive database from the list. 

  SYNOPSIS
    clear_dbs()
    s_dbs            The list of sensitive databases.
*/
void clear_dbs(sensitive_db_head* s_dbs);



#endif

