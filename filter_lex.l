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
#include <string.h>
#include <setjmp.h>
#include "filter_parser.h"
#include "filter.h"

int line_no = 1;
jmp_buf parser_error_env;

#define YY_FATAL_ERROR(msg) \
	do { \
		fprintf(stderr, "%s\n", msg); \
		longjmp(parser_error_env, 1); \
	} while(0);
%}

%option nounput noinput

%%
\n	{line_no++;}
[ \t]	{/* DO NOTHING */}
^#.*[ \t]*  {/* Comments */}
"database"	{return DATABASE;}
"table"		{return TABLE;}
[a-zA-Z0-9_\-]+ {yylval.name = strdup(yytext); return ID;}
\"[^"]*\" {yylval.name = strdup(yytext+1); yylval.name[strlen(yylval.name)-1] = '\0'; return ID; }
\'[^']*\' {yylval.name = strdup(yytext+1); yylval.name[strlen(yylval.name)-1] = '\0'; return ID; }
.	{return yytext[0];}
%%

