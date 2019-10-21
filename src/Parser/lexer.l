%{
    #include <stdio.h>
    #include <iostream>
    #include <sstream>
    #include <string>
    #include "../../src/Parser/Nodes/ConstraintNode.h"
    #include "../../src/Parser/Nodes/VariableNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/CreateNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/BaseValueNode.h"
    #include "../../src/Parser/Nodes/ColumnNode.h"

    #include "parser.h"
    #include <memory>
    #include <cstring>
    void showError();
    void yyerror(const char *s);
    int ch;
    std::string charLen;
%}

%option noinput noyywrap case-insensitive yylineno

NUMBER   ([-0-9])*
FLOATNUM ([0-9]+"."[0-9]+)+
IDENT    ([A-Za-z0-9]*)
STRVAL   (\'[-a-zA-Z0-9]*\')
CHAR     "CHAR("[-0-9]*")"

%%
"CREATE TABLE"          {ch += yyleng; return CREATE_TABLE_ACTION;}
"SHOW CREATE TABLE"     {ch += yyleng; return SHOWCREATE_TABLE_ACTION;}
"DROP TABLE"            {ch += yyleng; return DROP_TABLE_ACTION;}
"INSERT INTO"           {ch += yyleng; return INSERT_INTO_ACTION;}
"DELETE FROM"           {ch += yyleng; return DELETE_FROM_ACTION;}
"VALUES"                {ch += yyleng; return VALUES;}
"SELECT"                {ch += yyleng; return SELECT_ACTION;}
"FROM"                  {ch += yyleng; return FROM;}
"UPDATE"                {ch += yyleng; return UPDATE_ACTION;}
"SET"                   {ch += yyleng; return SET;}
"AS"                    {ch += yyleng; return AS;}
"WHERE"                 {ch += yyleng; return WHERE;}
"NOT"                   {ch += yyleng; return NOT;}
"AND"                   {ch += yyleng; return AND;}
"OR"                    {ch += yyleng; return OR;}
"("                     {ch += yyleng; return LBRACKET;}
")"                     {ch += yyleng; return RBRACKET;}
"="                     {ch += yyleng; return EQUAL;}
"*"                     {ch += yyleng; return STAR;}
"/"                     {ch += yyleng; return DIV;}
"+"                     {ch += yyleng; return PLUS;}
"-"                     {ch += yyleng; return MINUS;}
">"                     {ch += yyleng; return MORE;}
"<"                     {ch += yyleng; return LESS;}
">="                    {ch += yyleng; return MOREEQ;}
"<="                    {ch += yyleng; return LESSEQ;}
"!="                    {ch += yyleng; return NOTEQ;}
","                     {ch += yyleng; return COMMA;}
";"                     {ch += yyleng; return SEMICOLON;}
"."                     {ch += yyleng; return DOT;}
"INT"                   {ch += yyleng; return INT;}
"FLOAT"                 {ch += yyleng; return FLOAT;}
"NULL"                  {ch += yyleng; return VALNULL;}
"UNIQUE"                {ch += yyleng; return CONSTR_UNIQUE;}
"NOT NULL"              {ch += yyleng; return CONSTR_NOT_NULL;}
"PRIMARY KEY"           {ch += yyleng; return CONSTR_PRIMARY_KEY;}
{CHAR}                  {ch += yyleng;
                        charLen = "";
                        for (int i = 0; i < yyleng; i++) {
                            if(isdigit(yytext[i])) {
                                charLen += yytext[i];
                            }
                        }
                        try {
                            yylval.charLen = std::stoi(charLen);
                        } catch(...) {
                            yyerror("Char oversize");
                            return 1;
                        }
                        return CHAR;}
{NUMBER}                {ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return NUMBER; }
{FLOATNUM}              {ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return FLOATNUM; }
{STRVAL}                {ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return STRVAL; }
{IDENT}                 {ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return IDENT; }

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
