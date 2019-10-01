%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <iostream>
    #include <string>
    #include "../../src/Logic/Headers/MainLogic.h"
    MainLogic logicApi;

    int yylex();
    int yyerror(char *s);
%}


%token STRING OTHER SEMICOLON COMMA DDLCREATE DDLSHOW DDLDROP TABLE BRACKET TYPE CONSTRAINT

%type <string> STRING
%type <string> DDLCREATE
%type <string> DDLSHOW
%type <string> DDLDROP
%type <table> TABLE
%type <brackets> BRACKET
%type <string> TYPE
%type <string> CONSTRAINT

%union{
    char string[20];
    char table[5];
    char brackets[1];
}

%%

request:
    ddl_actions SEMICOLON {
    	logicApi.finish();
    }
    | request request
;

ddl_actions:
    DDLCREATE table {
        logicApi.addActionName($1);
    }
    | DDLDROP table{
        logicApi.addActionName($1);
    }
    | DDLSHOW table{
        logicApi.addActionName($1);
    };

table:
    TABLE STRING{
        logicApi.addTableName($2);
    }
    |table brackets;

brackets:
    BRACKET inner_expr BRACKET
;

inner_expr:
    STRING TYPE {
	logicApi.addColumn($1, $2);
    }
    | inner_expr CONSTRAINT {
    	logicApi.addConstraint($2);
    }
    | inner_expr COMMA inner_expr;


%%

void set_input_string(const char* in);
void end_string_scan(void);


int parse_request(const char* in) {
  set_input_string(in);
  int res = yyparse();
  end_string_scan();
  return res;
}

int yyerror(char *s){
    printf("Syntax Error on line %s\n", s);
    return 0;
}