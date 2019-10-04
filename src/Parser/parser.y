%{
    #define YYERROR_VERBOSE
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <string>
    #include "../../src/Logic/Headers/MainLogic.h"
    #include "../../src/Utils/Headers/CommonUtils.h"
    MainLogic logicApi;

    int yylex();
    int yyerror(const char *s);
    Response response;
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
    	response = logicApi.finish();
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


Response parse_request(const char* in) {
  Response temp;
  response = temp;

  set_input_string(in);
  int res = yyparse();
  end_string_scan();
  response.code = response.code || res;
  return response;
}

int yyerror(const char *s){
    response.errorMsg = s;
    printf("%s\n", s);
    return 0;
}