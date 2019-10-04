%{
    #include <stdio.h>
    #include <string.h>
    #include "parser.h"
    void showError();
    int ch;
%}

%option noinput noyywrap case-insensitive yylineno


numbers ([0-9])+
words   ([A-Za-z][A-Za-z0-9]*)
ddlcreate "CREATE"
ddldrop "DROP"
ddlshow "SHOW CREATE"
types "INT"|"FLOAT"|"CHAR"|"BOOLEAN"

brackets "("|")"
table "TABLE"|"table"
constraints "UNIQUE"|"NOT NULL"|"PRIMARY KEY"

%%
{ddldrop}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLDROP; }
{ddlcreate}     { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLCREATE; }
{ddlshow}       { ch += yyleng; sscanf(yytext, "%s", yylval.string); return DDLSHOW; }
{table}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return TABLE; }
{types}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return TYPE; }
{brackets}      { ch += yyleng; sscanf(yytext, "%c", yylval.string); return BRACKET; }
{constraints}   { ch += yyleng; sscanf(yytext, "%s", yylval.string); return CONSTRAINT; }
{words}         { ch += yyleng; sscanf(yytext, "%s", yylval.string); return STRING; }
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