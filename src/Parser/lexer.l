%{
    #include <stdio.h>
    #include <string>
    #include "parser.h"
    #include <memory>
    #include <cstring>
    void showError();
    int ch;
%}

%option noinput noyywrap case-insensitive yylineno

all "*"
numbers ([-0-9])+
floatnumb ([0-9]+"."[0-9]+)+
words   ([A-Za-z0-9]*)
valstr (\'[-a-zA-Z0-9]*\')
ddlcreate "CREATE TABLE"
ddldrop "DROP TABLE"
ddlshow "SHOW CREATE TABLE"
types "INT"|"FLOAT"|"CHAR"
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
notequally "!="
sign "+"|"-"
div "/"
comp ">="|"<="|"<"|">"
logic "and"|"or"
not "NOT"
brackets "("|")"
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
{types}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return TYPE; }
{set}           { ch += yyleng; sscanf(yytext, "%s", yylval.string); return SET; }
{brackets}      { ch += yyleng; sscanf(yytext, "%s", yylval.string); return BRACKET; }
{equally}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return EQUALLY; }
{notequally}    { ch += yyleng; sscanf(yytext, "%s", yylval.string); return NOTEQUALLY; }
{sign}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return SIGN; }
{div}           { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DIV; }
{comp}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return COMP; }
{logic}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return LOGIC; }
{not}           { ch += yyleng; sscanf(yytext, "%s", yylval.string); return NOT; }
{constraints}   { ch += yyleng; sscanf(yytext, "%s", yylval.string); return CONSTRAINT; }
{from}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return FROM; }
{where}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return WHERE; }
{numbers}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return NUMBER; }
{floatnumb}     { ch += yyleng; sscanf(yytext, "%s", yylval.string); return FLOATNUM; }
{null}          { ch += yyleng; sscanf(yytext, "%s", yylval.string); return VALNULL; }
{words}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return STRING; }
{valstr}        { ch += yyleng; sscanf(yytext, "%s", yylval.string); return VALSTR; }
{other}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return OTHER; }
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