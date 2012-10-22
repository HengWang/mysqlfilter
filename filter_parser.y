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

%{
#include <stdlib.h>
#include <stdio.h>
#include "filter.h"

int yylex(void);
extern int line_no;
extern char *yytext;

extern sensitive_db_head* sensitive_filter;
static sensitive_table_head* s_tables = NULL;
static sensitive_field_head* f_s_fields = NULL;
static sensitive_field_head* w_s_fields = NULL;

static void yyerror(const char *s)
{
    fprintf(stderr, "error at line number %d at %s:%s\n", 
        line_no, yytext,s);
}

int yywrap(void)
{
    return 1;
}

%}

%token ID DATABASE TABLE FILTER WHERE LINK

%union {
    char *name;
    int val;
}
%type <name> ID LINK
%type <val> start database table filter_or_where  
%type <val> filter where filter_field where_field
%start start
%%

start   : start database
    {
        $$ = $1;
    }
    |
    {
        $$ = 1;
    }
        ;
database   :       DATABASE ID '{' table '}'
    {
        sensitive_db* s_db = NULL;
        if($4 < 0)
        {
            free($2);
            if(s_tables){            
                uninit_table_head(s_tables);
				s_tables = NULL;
            }
            fprintf(stderr, "!!! The table module failed when parsing "
                "at line number %d, the name of db: %s\n", 
                line_no, $2);
            $$ =  -5;
            return $$;
        }
        if(init_db(&s_db, $2, s_tables)){
            free($2);
            fprintf(stderr, "!!! Initialize the database failed when "
                "parsing at line number %d, the name of db: %s\n", 
                line_no, $2);
            $$ = -5;
            return $$;
        }
        if(add_db(sensitive_filter, s_db)){
            free($2);
            if(s_db){
				uninit_db(s_db);
				s_db = NULL;
            }
            fprintf(stderr, "!!! Add database failed when parsing at "
                "line number %d, the name of db: %s\n", 
                line_no, $2);
            $$ = -5;
            return $$;
        }
        s_tables = (sensitive_table_head*)NULL;
        free($2);
        $$ = 1;
    }
        ;

table
        :     TABLE  ID '{' filter_or_where '}'
    {
        sensitive_table* s_table = NULL;
        if($4<0){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The filter or where module failed when "
                "parsing at line number %d, the name of table: %s\n", 
                line_no, $2);
            $$ =  -4;
            return $$;
        }
        if(init_table(&s_table, $2, f_s_fields, w_s_fields)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the table where field lists failed "
                "when parsing at line number %d, the name of table: %s\n", 
                line_no, $2);
            $$ = -4;
            return $$;
        }     
        if(!s_tables && init_table_head(&s_tables)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            fprintf(stderr, "!!! Initialize table head failed when parsing "
                "at line number %d, the name of table: %s\n",  
                line_no, $2);
            $$ = -4;
            return $$;          
        }
        if(add_table(s_tables, s_table)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            if(s_tables){
				uninit_table_head(s_tables);
				s_tables = NULL;
            }
            fprintf(stderr, "!!! Add table failed when parsing at line " 
                "number  %d, the name of table: %s\n",    
                line_no, $2);
            $$ = -4;
            return $$;
        }
        f_s_fields = NULL;
        w_s_fields = NULL;
        free($2);
        $$ = 1;
    }
     |       table TABLE  ID '{' filter_or_where '}'
    {
        sensitive_table* s_table = NULL;
        if($1 < 0 || $5<0){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The filter or where module failed when parsing "
                "at line number %d, the name of table: %s\n",
                line_no, $3);
            $$ = -4;
            return $$;
        }
        if(init_table(&s_table, $3, f_s_fields, w_s_fields)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the table where field lists failed "
                "when parsing at line number %d, the name of table: %s\n", 
                line_no, $3);
            $$ = -4;
            return $$;
        }      
        if(!s_tables && init_table_head(&s_tables)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            fprintf(stderr, "!!! Initialize table head failed when parsing "
                "at line number %d, the name of table: %s\n",  
                line_no, $3);
            $$ = -4;
            return $$;          
        }
        if(add_table(s_tables, s_table)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            if(s_tables){
				uninit_table_head(s_tables);
				s_tables = NULL;
            }
            fprintf(stderr, "!!! Add table failed when parsing at line " 
                "number  %d, the name of table: %s\n",    
                line_no, $3);
            $$ = -4;
            return $$;
        }
        f_s_fields = NULL;
        w_s_fields = NULL;
        free($3);
        $$ = 1;
    }
		|     TABLE  ID '{' filter '}'
    {
        sensitive_table* s_table = NULL;
        if($4<0){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The filter or where module failed when "
                "parsing at line number %d, the name of table: %s\n", 
                line_no, $2);
            $$ =  -4;
            return $$;
        }
        if(init_table(&s_table, $2, f_s_fields, w_s_fields)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the table where field lists failed "
                "when parsing at line number %d, the name of table: %s\n", 
                line_no, $2);
            $$ = -4;
            return $$;
        }     
        if(!s_tables && init_table_head(&s_tables)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            fprintf(stderr, "!!! Initialize table head failed when parsing "
                "at line number %d, the name of table: %s\n",  
                line_no, $2);
            $$ = -4;
            return $$;          
        }
        if(add_table(s_tables, s_table)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            if(s_tables){
				uninit_table_head(s_tables);
				s_tables = NULL;
            }
            fprintf(stderr, "!!! Add table failed when parsing at line " 
                "number  %d, the name of table: %s\n",    
                line_no, $2);
            $$ = -4;
            return $$;
        }
        f_s_fields = NULL;
        w_s_fields = NULL;
        free($2);
        $$ = 1;
    }
     |       table TABLE  ID '{' filter '}'
    {
        sensitive_table* s_table = NULL;
        if($1 < 0 || $5<0){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The filter or where module failed when parsing "
                "at line number %d, the name of table: %s\n",
                line_no, $3);
            $$ = -4;
            return $$;
        }
        if(init_table(&s_table, $3, f_s_fields, w_s_fields)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the table where field lists failed "
                "when parsing at line number %d, the name of table: %s\n", 
                line_no, $3);
            $$ = -4;
            return $$;
        }      
        if(!s_tables && init_table_head(&s_tables)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            fprintf(stderr, "!!! Initialize table head failed when parsing "
                "at line number %d, the name of table: %s\n",  
                line_no, $3);
            $$ = -4;
            return $$;          
        }
        if(add_table(s_tables, s_table)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            if(s_tables){
				uninit_table_head(s_tables);
				s_tables = NULL;
            }
            fprintf(stderr, "!!! Add table failed when parsing at line " 
                "number  %d, the name of table: %s\n",    
                line_no, $3);
            $$ = -4;
            return $$;
        }
        f_s_fields = NULL;
        w_s_fields = NULL;
        free($3);
        $$ = 1;
    }
		|     TABLE  ID '{' where '}'
    {
        sensitive_table* s_table = NULL;
        if($4<0){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The filter or where module failed when "
                "parsing at line number %d, the name of table: %s\n", 
                line_no, $2);
            $$ =  -4;
            return $$;
        }
        if(init_table(&s_table, $2, f_s_fields, w_s_fields)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the table where field lists failed "
                "when parsing at line number %d, the name of table: %s\n", 
                line_no, $2);
            $$ = -4;
            return $$;
        }     
        if(!s_tables && init_table_head(&s_tables)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            fprintf(stderr, "!!! Initialize table head failed when parsing "
                "at line number %d, the name of table: %s\n",  
                line_no, $2);
            $$ = -4;
            return $$;          
        }
        if(add_table(s_tables, s_table)){
            free($2);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            if(s_tables){
				uninit_table_head(s_tables);
				s_tables = NULL;
            }
            fprintf(stderr, "!!! Add table failed when parsing at line " 
                "number  %d, the name of table: %s\n",    
                line_no, $2);
            $$ = -4;
            return $$;
        }
        f_s_fields = NULL;
        w_s_fields = NULL;
        free($2);
        $$ = 1;
    }
     |       table TABLE  ID '{' where '}'
    {
        sensitive_table* s_table = NULL;
        if($1 < 0 || $5<0){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The filter or where module failed when parsing "
                "at line number %d, the name of table: %s\n",
                line_no, $3);
            $$ = -4;
            return $$;
        }
        if(init_table(&s_table, $3, f_s_fields, w_s_fields)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the table where field lists failed "
                "when parsing at line number %d, the name of table: %s\n", 
                line_no, $3);
            $$ = -4;
            return $$;
        }      
        if(!s_tables && init_table_head(&s_tables)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            fprintf(stderr, "!!! Initialize table head failed when parsing "
                "at line number %d, the name of table: %s\n",  
                line_no, $3);
            $$ = -4;
            return $$;          
        }
        if(add_table(s_tables, s_table)){
            free($3);
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            if(s_table){
				uninit_table(s_table);
				s_table = NULL;
            }
            if(s_tables){
				uninit_table_head(s_tables);
				s_tables = NULL;
            }
            fprintf(stderr, "!!! Add table failed when parsing at line " 
                "number  %d, the name of table: %s\n",    
                line_no, $3);
            $$ = -4;
            return $$;
        }
        f_s_fields = NULL;
        w_s_fields = NULL;
        free($3);
        $$ = 1;
    }
        ;

filter_or_where
         :      WHERE '{' where_field '}' filter
    {
        if($3 < 0 || $5 < 0){
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The field module or filter grammar failed "
                "when parsing at line number %d\n",
                line_no);
            $$ = -3;
            return $$;
        }
        $$ = 1;
    }        
         |      FILTER '{' filter_field '}' where
    {
        if($3 < 0 || $5 < 0){
            if(w_s_fields){          
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            if(f_s_fields){          
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The field module or where grammar failed "
                "when parsing at line number %d\n",
                line_no);
            $$ = -3;
            return $$;
        }
        $$ = 1;
    }      
        ;
        
filter
        :      FILTER '{' filter_field '}'
    {
        $$ = $3;
        if($$<0){
            if(f_s_fields) {           
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The field module failed when parsing at "
                "line number %d\n", line_no);
            $$ = -2;
            return $$;
        }
    }
        ;
        
where
        :      WHERE '{' where_field '}' 
    {
        $$ = $3;
        if($$<0){
            if(w_s_fields){            
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            fprintf(stderr, "!!! The field module failed when parsing at "
                "line number %d\n", line_no);
            $$ = -2;
            return $$;
        }       
    }
        ;

filter_field
        :       ID LINK ID ';'
    {
        sensitive_field* s_field = NULL;
        if (init_field(&s_field, $1, $2, $3)) {
            free($1);free($2);free($3); 
            fprintf(stderr, "!!! Initialize the field failed when parsing at " 
                "line number %d, the key: %s %s value: %s\n",    
                line_no, $1, $2, $3);           
            $$ =  -1;
            return $$;
        }
        if(!f_s_fields && init_field_head(&f_s_fields,FILTER_TYPE)){
            free($1);free($2);free($3); 
            if(s_field){
				uninit_field(s_field);
				s_field = NULL;
            }
            fprintf(stderr, "!!! Initialize the field head failed when parsing " 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $1, $2, $3);
            $$ =  -1;
            return $$;
        }
        if(add_field(f_s_fields,s_field)){
            free($1);free($2);free($3); 
            if(s_field){
				uninit_field(s_field);
				s_field = NULL;
            }
            if(f_s_fields){
				uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the field head failed when parsing" 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $1, $2, $3);
            $$ = -1;
            return $$;
        }
        free($1);free($2);free($3); 
        $$ = 1;
    }
         |       filter_field ID LINK ID ';'
    {
        sensitive_field* s_field = NULL;
        $$ = $1;
        if($$ < 0){
            free($2);free($3);free($4);
            if(f_s_fields) {           
                uninit_field_head(f_s_fields);
                f_s_fields = NULL;
            }
            fprintf(stderr, "!!! The field module failed when parsing at " 
                "line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);   
            $$ = -1;
            return $$;
        }
        if (init_field(&s_field, $2, $3, $4)) {
            free($2);free($3);free($4); 
            if(f_s_fields){
                uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the field failed when parsing at " 
                "line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);
            $$ = -1;
            return $$;
        }
        if(!f_s_fields && init_field_head(&f_s_fields,FILTER_TYPE)){
            free($2);free($3);free($4); 
            if(s_field){
				uninit_field(s_field);
				s_field = NULL;
            }
            fprintf(stderr, "!!! Initialize the field head failed when parsing " 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);
            $$ =  -1;
            return $$;
        }
        if(add_field(f_s_fields,s_field)){
            free($2);free($3);free($4); 
            if(s_field){
				uninit_field(s_field);
				s_field = NULL;
            }
            if(f_s_fields){
				uninit_field_head(f_s_fields);
				f_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the field head failed when parsing" 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);
            $$ = -1;
            return $$;
        }
        free($2);free($3);free($4); 
        $$ = 1;
    }   
        ;
		
where_field
        :       ID LINK ID ';'
    {
        sensitive_field* s_field = NULL;
        if (init_field(&s_field, $1, $2, $3)) {
            free($1);free($2);free($3); 
            fprintf(stderr, "!!! Initialize the field failed when parsing at " 
                "line number %d, the key: %s %s value: %s\n",    
                line_no, $1, $2, $3);           
            $$ =  -1;
            return $$;
        }
        if(!w_s_fields && init_field_head(&w_s_fields,WHERE_TYPE)){
            free($1);free($2);free($3); 
            if(s_field){
				uninit_field(s_field);
				s_field = NULL;
            }
            fprintf(stderr, "!!! Initialize the field head failed when parsing " 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $1, $2, $3);
            $$ =  -1;
            return $$;
        }
        if(add_field(w_s_fields,s_field)){
            free($1);free($2);free($3); 
            uninit_field(s_field);
            s_field = NULL;
            uninit_field_head(w_s_fields);
            w_s_fields = NULL;
            fprintf(stderr, "!!! Initialize the field head failed when parsing" 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $1, $2, $3);
            $$ = -1;
            return $$;
        }
        free($1);free($2);free($3); 
        $$ = 1;
    }
         |       where_field ID LINK ID ';'
    {
        sensitive_field* s_field = NULL;
        $$ = $1;
        if($$<0){
            free($2);free($3);free($4);
            if(w_s_fields){            
                uninit_field_head(w_s_fields);
                w_s_fields = NULL;
            }
            fprintf(stderr, "!!! The field module failed when parsing at " 
                "line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);   
            $$ = -1;
            return $$;
        }
        if (init_field(&s_field, $2, $3, $4)) {
            free($2);free($3);free($4); 
            if(w_s_fields){
                uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the field failed when parsing at " 
                "line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);
            $$ = -1;
            return $$;
        }
        if(!w_s_fields && init_field_head(&w_s_fields,WHERE_TYPE)){
            free($2);free($3);free($4); 
            if(s_field){
				uninit_field(s_field);
				s_field = NULL;
            }
            fprintf(stderr, "!!! Initialize the field head failed when parsing " 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);
            $$ =  -1;
            return $$;
        }
        if(add_field(w_s_fields,s_field)){
            free($2);free($3);free($4); 
             if(s_field){
				uninit_field(s_field);
				s_field = NULL;
            }
            if(w_s_fields){
				uninit_field_head(w_s_fields);
				w_s_fields = NULL;
            }
            fprintf(stderr, "!!! Initialize the field head failed when parsing" 
                "at line number %d, the key: %s %s value: %s\n",    
                line_no, $2, $3, $4);
            $$ = -1;
            return $$;
        }
        free($2);free($3);free($4); 
        $$ = 1;
    }   
        ;
%%
