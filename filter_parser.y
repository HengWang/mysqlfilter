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

static void yyerror(const char *s)
{
	fprintf(stderr, "error at line number %d at %s:%s\n", line_no, yytext,
		s);
}

int yywrap(void)
{
	return 1;
}

%}

%token ID DATABASE TABLE

%union {
	char *name;
	int val;
	struct st_sensitive_table_fields_head* table_head;
	struct st_key_value_head* field_head;
}
%type <name> ID
%type <val> start database 
%type <table_head>  table
%type <field_head> field
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
		if(init_db(&s_db, $2, $4)){
			free($2);
			fprintf(stderr, "initialize the db failed when parsing at line number %d, the name of db: %s\n",	line_no, $2);
			$$ = 0;
			return $$;
		}
		if(add_db(sensitive_filter, s_db)){
			free($2);
			uninit_db(s_db);
			fprintf(stderr, "add database failed when parsing at line number %d, the name of db: %s\n",	line_no, $2);
			$$ = 0;
			return $$;
		}
		s_tables = NULL;
		free($2);
		$$ = 1;
	}
	    ;

table
        :     TABLE  ID '{' field '}'
	{
		sensitive_table* s_table = NULL;
		if(init_table(&s_table, $2, $4)){
			free($2);
			fprintf(stderr, "initialize the table failed when parsing at line number %d, the name of db: %s\n",	line_no, $2);
			$$ = (sensitive_table_head*) NULL;
			return $$;
		}		
		if(!s_tables && init_table_head(&s_tables)){
			free($2);
			uninit_table(s_table);
			fprintf(stderr, "malloc the sensitive table head failed when parsing at line number %d\n",	line_no);
			$$ =  0;
			return $$;		    
		}
		if(add_table(s_tables, s_table)){
			free($2);
			uninit_table(s_table);
			uninit_table_head(s_tables);
			fprintf(stderr, "add table failed when parsing at line number %d, the name of db: %s\n",	line_no, $2);
			$$ =  (sensitive_table_head*) NULL;
			return $$;
		}
		free($2);
		$$ = s_tables;
	}
	 |       table TABLE  ID '{' field '}'
	{
		sensitive_table* s_table = NULL;
		sensitive_table_head* s_tables = $1;
		if(init_table(&s_table, $3, $5)){
			free($3);
			uninit_table_head(s_tables);
			fprintf(stderr, "initialize the table failed when parsing at line number %d, the name of db: %s\n",	line_no, $3);
			$$ =  (sensitive_table_head*) NULL;
			return $$;
		}
		if(add_table(s_tables, s_table)){
			free($3);
			uninit_table(s_table);
			uninit_table_head(s_tables);
			fprintf(stderr, "add table failed when parsing at line number %d, the name of db: %s\n",	line_no, $3);
			$$ =  (sensitive_table_head*) NULL;
			return $$;
		}
		free($3);
		$$ = s_tables;
	}
	    ;

field
        :       ID '=' ID ';'
	{
		sensitive_field* s_field = NULL;
		sensitive_field_head* s_fields = NULL;
		if (init_field(&s_field, $1, $3)) {
			free($1);free($3);			
			fprintf(stderr, "initialize the field failed when parsing at line number %d, the key: %s value: %s\n",
				line_no, $1, $3);
			$$ =  (sensitive_field_head*) NULL;
			return $$;
		}
		if(!s_fields && init_field_head(&s_fields)){
			free($1);free($3);	
			uninit_field(s_field);
			fprintf(stderr, "malloc sensitive field head failed when parsing at line number %d\n",	line_no);
			$$ =  (sensitive_field_head*) NULL;
			return $$;
		}
		if(add_field(s_fields,s_field)){
			free($1);free($3);	
			uninit_field(s_field);
			uninit_field_head(s_fields);
			fprintf(stderr, "add the field failed when parsing at line number %d, the field key: %s value: %s\n",
				line_no, $1, $3);
			$$ = (sensitive_field_head*) NULL;
			return $$;
		}
		free($1);free($3);	
		$$ = s_fields;
	}
	        |       field ID '=' ID ';'
	{
		sensitive_field* s_field = NULL;
		sensitive_field_head* s_fields = NULL;
		s_fields = $1;
		if (init_field(&s_field, $2, $4)) {
			free($2);free($4);	
			uninit_field_head(s_fields);
			fprintf(stderr, "initialize the field failed when parsing at line number %d, the key: %s value: %s\n",
				line_no, $2, $4);
			$$ = (sensitive_field_head*) NULL;
			return $$;
		}
		if(add_field(s_fields,s_field)){
			free($2);free($4);	
			uninit_field(s_field);
			uninit_field_head(s_fields);
			fprintf(stderr, "add the field failed when parsing at line number %d, the field key: %s value: %s\n",
				line_no, $2, $4);
			$$ = (sensitive_field_head*) NULL;
			return $$;
		}
		free($2);free($4);	
		$$ = s_fields;
	}
        ;
%%
