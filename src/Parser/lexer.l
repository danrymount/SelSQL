%{
    #include <stdio.h>
    #include <string>
    #include "parser.h"
    void showError();
    int ch;
%}

%option noinput noyywrap case-insensitive yylineno


numbers ([0-9])+
words   ([*A-Za-z][A-Za-z0-9]*)
ddlcreate "CREATE"
ddldrop "DROP"
ddlshow "SHOW CREATE"
types "INT"|"FLOAT"|"CHAR"|"BOOLEAN"
insert "INSERT INTO"
select "SELECT"
values "VALUES"
where "WHERE"
from "FROM"

equally "="
brackets "("|")"
table "TABLE"|"table"
constraints "UNIQUE"|"NOT NULL"|"PRIMARY KEY"

%%
{ddldrop}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLDROP; }
{ddlcreate}     { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLCREATE; }
{ddlshow}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLSHOW; }
{insert}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DMLINSERT; }
{select}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DQLSELECT; }
{values}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return VALUES; }
{table}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return TABLE; }
{types}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return TYPE; }
{brackets}      { ch += yyleng; sscanf(yytext, "%c", yylval.string); return BRACKET; }
{equally}       { ch += yyleng; sscanf(yytext, "%c", yylval.string); return EQUALLY; }
{constraints}   { ch += yyleng; sscanf(yytext, "%s", yylval.string); return CONSTRAINT; }
{from}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return FROM; }
{where}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return WHERE; }
{words}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return STRING; }
{numbers}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return NUMBER; }
";"             { ch += yyleng; return SEMICOLON; }
","             { ch += yyleng; return COMMA; }

%%

void showError(){
    printf("Other input");
}

void set_input_string(const char* in) {
  yy_scan_string(in);
}

void end_string_scan(void) {
  yy_delete_buffer(YY_CURRENT_BUFFER);
}