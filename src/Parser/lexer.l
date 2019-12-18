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
    #include "../../src/Parser/Nodes/ExpressionsNodes/BaseExprNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/ExprNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/CmpNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/IndentNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/SourceJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/BaseJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/UnionIntersectNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/SelectNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/UnionIntersectListNode.h"
    #include "../../src/Parser/Nodes/BaseNode.h"

    #include "parser.h"
    #include <memory>
    #include <cstring>
    void showError();
    void yyerror(const char *s);
    int ch;
    std::string charLen;
    std::string str;
%}

%option noinput noyywrap case-insensitive yylineno

NUMBER   ([0-9]*)
FLOATNUM ([0-9]+"."[0-9]+)+
IDENT    ([A-Za-z0-9]*)
STRVAL   [']([^']|[']['])*[']
CHAR_TYPE     "CHAR"[ \t\r\f]*"("[ \t\r\f]*[0-9]*[ \t\r\f]*")"
DATE_TYPE     ([0-9]+"-"+[0-9]+"-"+[0-9]+" "+[0-9]+":"+[0-9]+)

%%
"CREATE"                {ch += yyleng; return CREATE_ACTION;}
"SHOW"                  {ch += yyleng; return SHOW_ACTION;}
"DROP"                  {ch += yyleng; return DROP_ACTION;}
"INSERT"                {ch += yyleng; return INSERT_ACTION;}
"DELETE"                {ch += yyleng; return DELETE_ACTION;}
"TABLE"                 {ch += yyleng; return TABLE;}
"INTO"                  {ch += yyleng; return INTO;}
"FROM"                  {ch += yyleng; return FROM;}
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
"INT"                   {ch += yyleng; return INT_TYPE;}
"FLOAT"                 {ch += yyleng; return FLOAT_TYPE;}
"NULL"                  {ch += yyleng; return VALNULL;}
"UNIQUE"                {ch += yyleng; return CONSTR_UNIQUE;}
"NOT NULL"              {ch += yyleng; return CONSTR_NOT_NULL;}
"PRIMARY KEY"           {ch += yyleng; return CONSTR_PRIMARY_KEY;}
"JOIN"                  {ch += yyleng; return JOIN;}
"LEFT"                  {ch += yyleng; return LEFT;}
"RIGHT"                 {ch += yyleng; return RIGHT;}
"FULL"                  {ch += yyleng; return FULL;}
"ON"                    {ch += yyleng; return ON;}
"UNION"                 {ch += yyleng; return UNION;}
"INTERSECT"             {ch += yyleng; return INTERSECT;}
"BEGIN"                 {ch += yyleng; return BEGIN_;}
"COMMIT"                {ch += yyleng; return COMMIT;}
"FOR"                   {ch += yyleng; return FOR;}
"SYSTEM"                {ch += yyleng; return SYSTEM;}
"TIME"                  {ch += yyleng; return TIME;}
"ALL"                   {ch += yyleng; return ALL;}
"TO"                    {ch += yyleng; return TO;}

{CHAR_TYPE}             {ch += yyleng;
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
                        return CHAR_TYPE; }
{NUMBER}                {ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return NUMBER; }
{FLOATNUM}              {ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return FLOATNUM; }
{STRVAL}                {ch += yyleng;
                        yylval.string = new char[100]();
                        str = "";
                        for (int i = 0; i < yyleng; i++) {
                            if (yytext[i] == '\'' && yytext[i+1] == '\'' && i != 0 && i != yyleng-2) {
                                str += yytext[i];
                                i += 1;
                            } else {
                                str += yytext[i];
                            }
                        }
                        yylval.string = &str[0]; return STRVAL; }
{IDENT}                 {ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return IDENT; }
{DATE_TYPE}             {ch += yyleng;
                        yylval.string = new char[100]();
                        yylval.string = yytext;
                        return DATE_TYPE;}

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
