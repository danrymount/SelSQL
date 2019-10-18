%{
    #include <stdio.h>
    #include <string>
    #include "../../src/Parser/Nodes/ConstraintNode.h"
    #include "../../src/Parser/Nodes/VariableNode.h"
    #include "../../src/Parser/Nodes/VariablesListNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/CreateNode.h"

    #include "parser.h"
    #include <memory>
    #include <cstring>
    void showError();
    int ch;
%}

%option noinput noyywrap case-insensitive yylineno


IDENT   ([A-Za-z0-9]*)

%%
"CREATE TABLE"          {return CREATE_TABLE;}
"SHOW CREATE TABLE"     {return SHOW_CREATE_TABLE;}
"DROP TABLE"            {return DROP_TABLE;}
"("                     {return LBRACKET;}
")"                     {return RBRACKET;}
","                     {return COMMA;}
"INT"                   {return INT;}
"FLOAT"                 {return FLOAT;}
"CHAR"                  {return CHAR;}
"UNIQUE"                {return CONSTR_UNIQUE;}
"NOT NULL"              {return CONSTR_NOT_NULL;}
"PRIMARY KEY"           {return CONSTR_PRIMARY_KEY;}
{IDENT}                 { ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return IDENT; }

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