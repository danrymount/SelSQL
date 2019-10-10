%{
    #include <stdio.h>
    #include <string>
    #include "parser.h"
    void showError();
    int ch;
%}

%option noinput noyywrap case-insensitive yylineno

all "*"
numbers ([0-9])+
floatnumb ([0-9]+"."[0-9]+)+
words   ([-A-Za-z0-9]*)
ddlcreate "CREATE"
ddldrop "DROP"
ddlshow "SHOW CREATE"
types "INT"|"FLOAT"|"CHAR"|"BOOLEAN"
insert "INSERT INTO"
select "SELECT"
values "VALUES"
where "WHERE"
from "FROM"
delete "DELETE FROM"
update "UPDATE"
set "SET"
null "NULL"


equally "="
sign ">="|"<="|"!="|"<"|">"
brackets "("|")"
table "TABLE"|"table"
constraints "UNIQUE"|"NOT NULL"|"PRIMARY KEY"
other [^\40()<>=',;a-zA-Z0-9]+

%%
{all}           { ch += yyleng; sscanf(yytext, "%s", yylval.string); return ALL; }
{ddldrop}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLDROP; }
{ddlcreate}     { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLCREATE; }
{ddlshow}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLSHOW; }
{insert}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DMLINSERT; }
{select}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DQLSELECT; }
{delete}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DMLDELETE; }
{update}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DMLUPDATE; }
{values}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return VALUES; }
{table}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return TABLE; }
{types}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return TYPE; }
{set}           { ch += yyleng; sscanf(yytext, "%s", yylval.string); return SET; }
{brackets}      { ch += yyleng; sscanf(yytext, "%c", yylval.string); return BRACKET; }
{equally}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return EQUALLY; }
{sign}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return SIGN; }
{constraints}   { ch += yyleng; sscanf(yytext, "%s", yylval.string); return CONSTRAINT; }
{from}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return FROM; }
{where}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return WHERE; }
{numbers}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return NUMBER; }
{floatnumb}     { ch += yyleng; sscanf(yytext, "%s", yylval.string); return FLOATNUM; }
{null}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return VALNULL; }
{words}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return STRING; }
{other}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return OTHER; }
";"             { ch += yyleng; return SEMICOLON; }
","             { ch += yyleng; return COMMA; }
"'"             { ch += yyleng; return STROKE; }
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