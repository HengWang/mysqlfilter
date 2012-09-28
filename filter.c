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
#include <m_string.h>
#include <m_ctype.h>
#include "filter.h"

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
char* get_value(const sensitive_field_head* s_fields, const char* key )
{
  sensitive_field* field;
  DBUG_ENTER("get_value");
  if(!s_fields || TAILQ_EMPTY(s_fields) || !key)
    DBUG_RETURN(NULL);
  TAILQ_FOREACH(field, s_fields,next){
    if (field && field->key && strncasecmp(field->key, key, max(strlen(key),strlen(field->key)))==0)
      DBUG_RETURN(field->value);
  }
  DBUG_RETURN(NULL);
}

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
int get_field(const sensitive_field_head* s_fields, sensitive_field** s_field,const char* key )
{
  DBUG_ENTER("get_field");
  if(!s_fields || TAILQ_EMPTY(s_fields) || !key)
     DBUG_RETURN(-1);
  TAILQ_FOREACH(*s_field, s_fields,next){
    if (*s_field && (*s_field)->key && strncasecmp((*s_field)->key, key, max(strlen(key),strlen((*s_field)->key)))==0)
      DBUG_RETURN(0);
  }
  DBUG_RETURN(-1);
}

/*
  Check the key whether in the sensitive field list or not. 

  SYNOPSIS
    is_field_exists()
    s_fields       The list of sensitive fields.
    key              The given key.

  RETURN VALUES
    TRUE          success;
    FALSE         failed.
*/
my_bool is_field_exists(const sensitive_field_head* s_fields, const char* key)
{
  sensitive_field* field;
  DBUG_ENTER("is_field_exists");
  if(!s_fields || TAILQ_EMPTY(s_fields) || !key)
    DBUG_RETURN(FALSE);
  TAILQ_FOREACH(field, s_fields,next){
    if (field && field->key && !strncasecmp(field->key, key, max(strlen(key),strlen(field->key))))
      DBUG_RETURN(TRUE);
  }
  DBUG_RETURN(FALSE);
}

/*
  Initialize the sensitive field head. 

  SYNOPSIS
    init_field_head()
    s_fields         The object of sensitive field head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_field_head(sensitive_field_head** s_fields)
{
  DBUG_ENTER("init_field_head");
  *s_fields =  (sensitive_field_head*) my_malloc(sizeof(sensitive_field_head), MYF(MY_WME));
  if(!(*s_fields))
  {
    DBUG_PRINT("error",("!!!error: Malloc the sensitive field head failed.\n"));
    DBUG_RETURN(-1);	    
  }
  TAILQ_INIT(*s_fields);
  DBUG_RETURN(0);
}

/*
  Uninitialize the sensitive field head. 

  SYNOPSIS
    uninit_field_head()
    s_fields         The object of sensitive field head.
*/
void uninit_field_head(sensitive_field_head* s_fields)
{
  DBUG_ENTER("uninit_field_head");
  if(s_fields)
  {
    clear_fields(s_fields);
    my_free(s_fields);
  }
  DBUG_VOID_RETURN;
}

/*
  Initialize the sensitive field. 

  SYNOPSIS
    init_field()
    s_field         The object of sensitive field.
    key              The given key.
    value            The value of the key.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_field(sensitive_field** s_field, const char* key, const char* value)
{
  DBUG_ENTER("init_field");
  if (!key || !value)
  {
    DBUG_PRINT("error",("!!!error: The key or value is NULL!\n"));
    DBUG_RETURN(-1);
  }
  *s_field = (sensitive_field*) my_malloc(sizeof(sensitive_field), MYF(MY_WME));
  if(s_field==NULL)
  {
    DBUG_PRINT("error",("!!!error: Malloc the struct of sensitive_field failed!\n"));
    DBUG_RETURN(-1);
  }
  if(((*s_field)->key = my_strdup(key,MYF(MY_FAE)))==NULL)
  {
    my_free(*s_field);
    DBUG_PRINT("error",("!!!error: Duplicate the key: %s failed!\n",key));
    DBUG_RETURN(-1);
  }
  if (((*s_field)->value = my_strdup(value,MYF(MY_FAE)))==NULL)
  {
    my_free((*s_field)->key);
    my_free(*s_field);
    DBUG_PRINT("error",("!!!error: Duplicate the value: %s failed!\n",value));
    DBUG_RETURN(-1);
  }
  DBUG_RETURN(0);
}

/*
  Uninitialize the sensitive field. 

  SYNOPSIS
    uninit_field()
    s_field         The object of sensitive field.
*/
void uninit_field(sensitive_field* s_field)
{
  DBUG_ENTER("uninit_field");
  if(s_field)
  {
    my_free(s_field->key);
    my_free(s_field->value);
    my_free(s_field);
  }
  DBUG_VOID_RETURN;
}

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
int add_field(sensitive_field_head* s_fields, sensitive_field* s_field)
{
  DBUG_ENTER("add_field");
  if(!s_fields || !s_field)
  {
    DBUG_PRINT("error",("!!!error: The sensitive field head  is NULL, or the value of field is NULL!"));
    DBUG_RETURN(-1);
  }
  if(!is_field_exists(s_fields,s_field->key))
    TAILQ_INSERT_TAIL(s_fields,s_field,next);  
  DBUG_RETURN(0);
}

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
int remove_field(sensitive_field_head* s_fields, sensitive_field* s_field)
{
  DBUG_ENTER("remove_field");
  if(!s_fields || TAILQ_EMPTY(s_fields))
  {
    DBUG_PRINT("error",("!!!error: The sensitive field head is NULL or is empty!"));
    DBUG_RETURN(-1);
  }
  if (s_field == NULL)
  {
    DBUG_PRINT("warning",("!!warning: The field is NULL!\n"));
    DBUG_RETURN(0);
  }
  TAILQ_REMOVE(s_fields, s_field, next);
  uninit_field(s_field);
  DBUG_RETURN(0);
}

/*
  Clear all of the sensitive field from the list. 

  SYNOPSIS
    clear_fields()
    s_fields       The list of sensitive fields.
*/
void clear_fields(sensitive_field_head* s_fields)
{
  sensitive_field* s_field;
  DBUG_ENTER("clear_fields");
  if(!s_fields)
    DBUG_VOID_RETURN;
  for (s_field = TAILQ_FIRST(s_fields); s_field != NULL; s_field = TAILQ_FIRST(s_fields))
    remove_field(s_fields,s_field);
  DBUG_VOID_RETURN;
}

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
int get_table(const sensitive_table_head* s_tables, sensitive_table** s_table, const char* tb)
{
  DBUG_ENTER("get_table");
  if(!s_tables || TAILQ_EMPTY(s_tables) || !tb)
    DBUG_RETURN(-1);
  TAILQ_FOREACH(*s_table,s_tables,next){
    if (*s_table && (*s_table)->tb_name && !strncasecmp((*s_table)->tb_name,tb,max(strlen(tb),strlen((*s_table)->tb_name))))
      DBUG_RETURN(0);
  }
  DBUG_RETURN(-1);
}

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
my_bool is_table_exists(const sensitive_table_head* s_tables, const char* tb)
{
  sensitive_table* s_table;
  DBUG_ENTER("is_table_exists");
  if(!s_tables || TAILQ_EMPTY(s_tables) || !tb)
    DBUG_RETURN(FALSE);
  TAILQ_FOREACH(s_table,s_tables,next){
    if (s_table && s_table->tb_name && !strncasecmp(tb,s_table->tb_name,max(strlen(tb),strlen(s_table->tb_name))))
      DBUG_RETURN(TRUE);
  }
  DBUG_RETURN(FALSE);
}

/*
  Initialize the sensitive table head. 

  SYNOPSIS
    init_field_head()
    s_tables         The object of sensitive table head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_table_head(sensitive_table_head** s_tables)
{
  DBUG_ENTER("init_table_head");
  *s_tables =  (sensitive_table_head*) my_malloc(sizeof(sensitive_table_head), MYF(MY_WME));
  if(!(*s_tables))
  {
    DBUG_PRINT("error",("!!!error: Malloc the sensitive table head failed.\n"));
    DBUG_RETURN(-1);	    
  }
  TAILQ_INIT(*s_tables);
  DBUG_RETURN(0);
}

/*
  Uninitialize the sensitive table head. 

  SYNOPSIS
    uninit_field_head()
    s_tables         The object of sensitive table head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
void uninit_table_head(sensitive_table_head* s_tables)
{
  DBUG_ENTER("uninit_table_head");
  if(s_tables)
  {
    clear_tables(s_tables);
    my_free(s_tables);
  }
  DBUG_VOID_RETURN;

}

/*
  Initialize the sensitive table based on given table name. 

  SYNOPSIS
    init_table()
    s_table         The object of sensitive table.
    tb                 The table name
    s_fields          The sensitive field lists.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_table(sensitive_table** s_table, const char* tb, sensitive_field_head* s_fields)
{
  DBUG_ENTER("init_field");
  if(!tb)
  {
    DBUG_PRINT("error",("!!!error: The name of table is NULL!\n"));
    DBUG_RETURN(-1);
  }
  *s_table = (sensitive_table*) my_malloc(sizeof(sensitive_table), MYF(MY_WME));
  if(!(*s_table))
  {
    DBUG_PRINT("error",("!!!error: Malloc the struct of sensitive_table failed!\n"));
    DBUG_RETURN(-1);
  }
  if(((*s_table)->tb_name = my_strdup(tb,MYF(MY_FAE)))==NULL)
  {
    my_free(*s_table);
    DBUG_PRINT("error",("!!!error: Duplicate the table name: %s failed!\n",tb));
    DBUG_RETURN(-1);
  }
  if (!s_fields)
  {
    if(init_field_head(&((*s_table)->field_lists)))
    {
      DBUG_PRINT("error",("!!!error: Malloc the sensitive field head failed!\n"));
      my_free((*s_table)->tb_name);
      my_free(*s_table);
      DBUG_RETURN(-1);
    }
    TAILQ_INIT((*s_table)->field_lists);
  }else
    (*s_table)->field_lists = s_fields;
  DBUG_RETURN(0);
}

/*
  Uninitialize the sensitive table . 

  SYNOPSIS
    uninit_table()
    s_table         The object of sensitive table.
*/
void uninit_table(sensitive_table* s_table)
{
  DBUG_ENTER("uninit_field");
  if(s_table)
  {
    my_free(s_table->tb_name);
    if(s_table->field_lists)
      clear_fields(s_table->field_lists);
    my_free(s_table);
  }
  DBUG_VOID_RETURN;
}

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
int add_table(sensitive_table_head* s_tables, sensitive_table* s_table)
{
  sensitive_table* ptr_table;
  sensitive_field* ptr_field;
  DBUG_ENTER("add_field");
  if(!s_tables || !s_table)
  {
    DBUG_PRINT("error",("!!!error: The sensitive table head is NULL, or value of table is NULL!"));
    DBUG_RETURN(-1);
  }
  if (!get_table(s_tables,&ptr_table,s_table->tb_name))
  {
    TAILQ_FOREACH(ptr_field,s_table->field_lists,next)
      add_field(ptr_table->field_lists,ptr_field);
  }
  else
    TAILQ_INSERT_TAIL(s_tables,s_table,next);
  DBUG_RETURN(0);
}

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
int remove_table(sensitive_table_head* s_tables, sensitive_table* s_table)
{
  DBUG_ENTER("remove_table");
  if(!s_tables || TAILQ_EMPTY(s_tables))
  {
    DBUG_PRINT("error",("!!!error: The sensitive table head is NULL or is empty!"));
    DBUG_RETURN(-1);
  }
  if (!s_table)
  {
    DBUG_PRINT("warning",("!!warning: The s_table is NULL!\n"));
    DBUG_RETURN(0);
  }
  TAILQ_REMOVE(s_tables, s_table, next);
  uninit_table(s_table);
  DBUG_RETURN(0);
}

/*
  Clear all of the sensitive table from the list. 

  SYNOPSIS
    clear_tables()
    s_tables       The list of sensitive tables.
*/
void clear_tables(sensitive_table_head* s_tables)
{
  sensitive_table* s_table;
  DBUG_ENTER("clear_fields");
  if(!s_tables)
    DBUG_VOID_RETURN;
  for (s_table = TAILQ_FIRST(s_tables); s_table != NULL; s_table = TAILQ_FIRST(s_tables))
    remove_table(s_tables,s_table);
  DBUG_VOID_RETURN;
}

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
int get_database(const sensitive_db_head* s_dbs, sensitive_db** s_db, const char* db)
{
  DBUG_ENTER("get_database");
  if(!s_dbs || TAILQ_EMPTY(s_dbs) || !db)
    DBUG_RETURN(-1);
  TAILQ_FOREACH(*s_db,s_dbs,next){
    if (*s_db && (*s_db)->db_name && !strncasecmp((*s_db)->db_name,db,max(strlen(db),strlen((*s_db)->db_name))))
      DBUG_RETURN(0);
  }
  DBUG_RETURN(-1);
}

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
my_bool is_db_exists(const sensitive_db_head* s_dbs, const char* db)
{
  sensitive_db* s_db;
  DBUG_ENTER("is_db_exists");
  if(!s_dbs || TAILQ_EMPTY(s_dbs) || !db)
    DBUG_RETURN(FALSE);
  TAILQ_FOREACH(s_db,s_dbs,next){
    if (s_db && s_db->db_name && !strncasecmp(db,s_db->db_name,max(strlen(db),strlen(s_db->db_name))))
      DBUG_RETURN(TRUE);
  }
  DBUG_RETURN(FALSE);
}

/*
  Initialize the sensitive database head. 

  SYNOPSIS
    init_db_head()
    s_dbs         The object of sensitive database head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_db_head(sensitive_db_head** s_dbs)
{
  DBUG_ENTER("init_db_head");
  *s_dbs =  (sensitive_db_head*) my_malloc(sizeof(sensitive_db_head), MYF(MY_WME));
  if(!(*s_dbs))
  {
    DBUG_PRINT("error",("!!!error: Malloc the sensitive database head failed.\n"));
    DBUG_RETURN(-1);	    
  }
  TAILQ_INIT(*s_dbs);
  DBUG_RETURN(0);
}

/*
  Uninitialize the sensitive database head. 

  SYNOPSIS
    uninit_db_head()
    s_dbs         The object of sensitive database head.

  RETURN VALUES
    0                  success;
    -1                 failed.
*/
void uninit_db_head(sensitive_db_head* s_dbs)
{
  DBUG_ENTER("uninit_db_head");
  if(s_dbs)
  {
    clear_dbs(s_dbs);
    my_free(s_dbs);
  }
  DBUG_VOID_RETURN;

}

/*
  Initialize the sensitive database of given database name. 

  SYNOPSIS
    init_db()
    s_db              The object of sensitive database.
    db                  The database name
    s_tables         The sensitive table lists.
 
  RETURN VALUES
    0                  success;
    -1                 failed.
*/
int init_db(sensitive_db** s_db, const char* db, sensitive_table_head* s_tables)
{
  DBUG_ENTER("init_db");
  if(!db)
  {
    DBUG_PRINT("error",("!!!error: The name of database is NULL!\n"));
    DBUG_RETURN(-1);
  }
  *s_db = (sensitive_db*) my_malloc(sizeof(sensitive_db), MYF(MY_WME));
  if(!(*s_db))
  {
    DBUG_PRINT("error",("!!!error: Malloc the struct of sensitive_db failed!\n"));
    DBUG_RETURN(-1);
  }
  if(((*s_db)->db_name = my_strdup(db,MYF(MY_FAE)))==NULL)
  {
    my_free(*s_db);
    DBUG_PRINT("error",("!!!error: Duplicate the database name: %s failed!\n",db));
    DBUG_RETURN(-1);
  }
  if(!s_tables)
  {
    if(init_table_head(&((*s_db)->table_lists)))
    {
      DBUG_PRINT("error",("!!!error: Malloc the sensitive table head failed!\n"));
      my_free((*s_db)->db_name);
      my_free(*s_db);
      DBUG_RETURN(-1);
    }
    TAILQ_INIT((*s_db)->table_lists);
  }else
    (*s_db)->table_lists = s_tables;
  DBUG_RETURN(0);
}

/*
  Uninitialize the sensitive database. 

  SYNOPSIS
    uninit_db()
    s_db              The object of sensitive database.
*/
void uninit_db(sensitive_db* s_db)
{
  DBUG_ENTER("uninit_db");
  if(s_db)
  {
    my_free(s_db->db_name);
    if(s_db->table_lists)
      clear_tables(s_db->table_lists);
    my_free(s_db);
  }
  DBUG_VOID_RETURN;
}

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
int add_db(sensitive_db_head* s_dbs, sensitive_db* s_db)
{
  sensitive_db* ptr_db;
  sensitive_table* ptr_table;
  DBUG_ENTER("add_db");
  if(!s_dbs || !s_db)
  {
    DBUG_PRINT("error",("!!!error: The sensitive table head is NULL, or value of table is NULL!"));
    DBUG_RETURN(-1);
  }
  if (!get_database(s_dbs,&ptr_db,s_db->db_name))
  {
    TAILQ_FOREACH(ptr_table,s_db->table_lists,next)
      add_table(ptr_db->table_lists,ptr_table);
  }
  else
    TAILQ_INSERT_TAIL(s_dbs,s_db,next);
  DBUG_RETURN(0);
}

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
int remove_db(sensitive_db_head* s_dbs, sensitive_db* s_db)
{
  DBUG_ENTER("remove_db");
  if(!s_dbs || TAILQ_EMPTY(s_dbs))
  {
    DBUG_PRINT("error",("!!!error: The sensitive table head is NULL or is empty!\n"));
    DBUG_RETURN(-1);
  }
  if (s_db == NULL)
  {
    DBUG_PRINT("warning",("!!warning: The s_db is NULL!\n"));
    DBUG_RETURN(0);
  }
  TAILQ_REMOVE(s_dbs, s_db, next);
  uninit_db(s_db);
  DBUG_RETURN(0);
}

/*
  Clear all of the sensitive database from the list. 

  SYNOPSIS
    clear_dbs()
    s_dbs            The list of sensitive databases.
*/
void clear_dbs(sensitive_db_head* s_dbs)
{
  sensitive_db* s_db;
  DBUG_ENTER("clear_dbs");
  if(!s_dbs)
    DBUG_VOID_RETURN;
  for (s_db = TAILQ_FIRST(s_dbs); s_db != NULL; s_db = TAILQ_FIRST(s_dbs))
    remove_db(s_dbs,s_db);
  DBUG_VOID_RETURN;
}




