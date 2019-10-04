%{
    #define YYERROR_VERBOSE
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <string>
    #include "../../src/Parser/Headers/ParserLogic.h"
    ParserLogic logicApi;

    extern int yylineno;
    extern int ch;
    extern char *yytext;
    int yylex();
    int yyerror(const char *s);
    Response response;
%}

%token <string> STRING OTHER SEMICOLON COMMA DDLCREATE DDLSHOW DDLDROP TABLE BRACKET TYPE CONSTRAINT

%union{
    char string[256];
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

int yyerror(const char *errmsg){
    response.errorMsg = errmsg;
    fprintf(stderr, "%s (Str num %d, sym num %d): %s\n", errmsg, yylineno, ch, yytext);
    return 0;
}