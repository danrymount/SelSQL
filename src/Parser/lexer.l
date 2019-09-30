%{
    #include <stdio.h>
    #include <string.h>
    #include "parser.h"
    void showError();
%}

%option noinput noyywrap


numbers ([0-9])+
words   ([A-Za-z][A-Za-z0-9]*)
ddlcreate "CREATE"|"create"
ddldrop "DROP"|"drop"
ddlshow "SHOW CREATE"|"show create"
types "INT"|"int"|"FLOAT"|"float"|"CHAR"|"char"|"BOOLEAN"|"boolean"

brackets "("|")"
table "TABLE"|"table"
constraints "UNIQUE"|"NOT NULL"|"PRIMARY KEY"

%%
{ddldrop}       { sscanf(yytext, "%s", yylval.string); return DDLDROP; }
{ddlcreate}     { sscanf(yytext, "%s", yylval.string); return DDLCREATE; }
{ddlshow}       { sscanf(yytext, "%s", yylval.string); return DDLSHOW; }
{table}         { sscanf(yytext, "%s", yylval.table); return TABLE; }
{types}         { sscanf(yytext, "%s", yylval.string); return TYPE; }
{brackets}      { sscanf(yytext, "%c", yylval.brackets); return BRACKET; }
{constraints}   { sscanf(yytext, "%s", yylval.string); return CONSTRAINT; }
{words}         { sscanf(yytext, "%s", yylval.string); return STRING; }
";"             { return SEMICOLON; }
","             { return COMMA; }

%%

void showError(){
    printf("Other input");
}