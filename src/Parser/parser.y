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
    BigResponse response;
%}

%token <string> STRING OTHER SEMICOLON COMMA DDLCREATE DDLSHOW DDLDROP TABLE BRACKET TYPE CONSTRAINT DMLINSERT VALUES
NUMBER WHERE EQUALLY STR FROM DQLSELECT

%union{
    char string[256];
}

%%

request:
    actions SEMICOLON {
    	response = logicApi.finish();
    }
    |
    request request
;

actions:
    DDLCREATE table {
        logicApi.addActionName($1);
    }
    |
    DDLDROP table{
        logicApi.addActionName($1);
    }
    |
    DDLSHOW table{
        logicApi.addActionName($1);
    }
    |
    DMLINSERT act_insert {
	logicApi.addActionName($1);
    }
    |
    table_select

table_select:
    col_select FROM STRING {
        logicApi.addTableName($3);
    }
    |
    table_select where

col_select:
    DQLSELECT STRING {
    	printf("COL = %s\n", $2);
    }
    |
    col_select COMMA STRING {
    	printf("COL = %s\n", $3);
    }

act_insert:
    insert
    |
    insert where

where:
    WHERE STRING EQUALLY STRING
    |
    WHERE STRING EQUALLY NUMBER

insert:
    STRING values {
	logicApi.addTableName($1);
    }
    | STRING BRACKET STRING {
    	logicApi.addTableName($1);
        printf("TABLE = %s COL = %s\n", $1, $3);
    }
    | insert COMMA STRING {
        printf("COL = %s\n", $3);
    }
    | insert BRACKET
    | insert values

table:
    TABLE STRING{
        logicApi.addTableName($2);
    }
    |table brackets;

values:
    VALUES BRACKET
    |
    values STRING {
    	printf("VAL = %s\n", $2);
    }
    |
    values NUMBER {
    	printf("VAL = %s\n", $2);
    }
    |
    values COMMA STRING  {
         printf("VAL = %s\n", $3);
    }
    |
    values COMMA NUMBER  {
         printf("VAL = %s\n", $3);
    }
    |
    values BRACKET

brackets:
    BRACKET inner_expr BRACKET

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
  ch = 0;
  response.clear();

  set_input_string(in);
  int res = yyparse();
  end_string_scan();
  response.code = response.code || res;
  return response;
}

int yyerror(const char *errmsg){
    response.errorMsg = std::string(errmsg) + " (Str num " + std::to_string(yylineno) + ", sym num " + std::to_string(ch) +"): "+ std::string(yytext);
    //fprintf(stderr, "%s (Str num %d, sym num %d): %s\n", errmsg, yylineno, ch, yytext);

    return 0;
}