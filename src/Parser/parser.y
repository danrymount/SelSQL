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
NUMBER WHERE EQUALLY STR FROM DQLSELECT DMLDELETE DMLUPDATE SET ALL STROKE VALNULL

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
    DMLINSERT insert_where {
    	logicApi.addActionName($1);
	printf("INSERT\n");
    }
    |
    table_select {
        printf("SELECT\n");
    }
    |
    table_delete {
        printf("DELETE\n");
    }
    |
    DMLUPDATE STRING table_update {
        printf("%s, %s\n", $1, $2);
    }

table_update:
    update_set
    |
    table_update where

update_set:
    SET STRING EQUALLY STROKE STRING STROKE {
        printf("UPCOL = %s, UPVALSTR = %s\n", $2, $5);
    }
    |
    SET STRING EQUALLY NUMBER {
        printf("UPCOL = %s, UPVALNUM = %s\n", $2, $4);
    }
    |
    SET STRING EQUALLY VALNULL {
        printf("UPCOL = %s, UPVALNULL = %s\n", $2, $4);
    }
    |
    SET STRING EQUALLY STROKE VALNULL STROKE {
        printf("UPCOL = %s, UPVALSTRNULL = %s\n", $2, $5);
    }
    |
    update_set COMMA STRING EQUALLY STROKE STRING STROKE {
    	printf("UPCOL = %s, UPVALSTR = %s\n", $2, $5);
    }
    |
    update_set COMMA STRING EQUALLY NUMBER {
        printf("UPCOL = %s, UPVALNUM = %s\n", $2, $4);
    }
    |
    update_set COMMA STRING EQUALLY STROKE VALNULL STROKE {
        printf("UPCOL = %s, UPVALSTR = %s\n", $2, $5);
    }
    |
    update_set COMMA STRING EQUALLY VALNULL {
        printf("UPCOL = %s, UPVALNULL = %s\n", $2, $4);
    }

table_delete:
    DMLDELETE STRING
    |
    DMLDELETE STRING where

table_select:
    col_select FROM STRING {
    	logicApi.addTableName($3);
        printf("TABLE = %s\n", $3);
    }
    |
    table_select where

col_select:
    DQLSELECT STRING {
    	logicApi.addSelectColumn($2);
    	printf("COL = %s\n", $2);
    }
    |
    DQLSELECT ALL {
        printf("COLALL = %s\n", $2);
    }
    |
    col_select COMMA STRING {
    	logicApi.addSelectColumn($2);
    	printf("COL = %s\n", $2);
    }
    |
    col_select COMMA ALL {
        printf("COLALL = %s\n", $2);
    }

insert_where:
    insert
    |
    insert where

where:
    WHERE STRING EQUALLY STRING {
        printf("WHERE %s %s %s\n", $2, $3, $4);
    }

insert:
    STRING values {
    	logicApi.addTableName($1);
	printf("TABLE = %s\n", $1);
    }
    | STRING BRACKET STRING {
    	logicApi.addTableName($1);
        logicApi.addColumn($3);
        printf("TABLE = %s COL = %s\n", $1, $3);
    }
    | insert COMMA STRING {
        logicApi.addColumn($3);
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
    values STROKE STRING STROKE {
    	logicApi.addValue($3);
    	printf("VALSTR = %s\n", $3);
    }
    |
    values STROKE VALNULL STROKE {
    	printf("VALSTRNULL = %s\n", $3);
    }
    |
    values NUMBER {
    	logicApi.addValue($2);
    	printf("VALNUM = %s\n", $2);
    }
    |
    values VALNULL {
    	printf("VALNULL = %s\n", $2);
    }
    |
    values COMMA STROKE STRING STROKE {
         printf("VALSTR = %s\n", $4);
    }
    |
    values COMMA STROKE VALNULL STROKE {
    	logicApi.addValue($4);
        printf("VALSTRNULL = %s\n", $4);
    }
    |
    values COMMA NUMBER {
    	logicApi.addValue($3);
        printf("VALNUM = %s\n", $3);
    }
    |
    values COMMA VALNULL {
        printf("VALNULL = %s\n", $3);
    }
    |
    values BRACKET

brackets:
    BRACKET inner_expr BRACKET

inner_expr:
    STRING TYPE {
	logicApi.addColumn($1, $2);
	printf("VA = %s\n", $1);
    }
    | inner_expr CONSTRAINT {
    	logicApi.addConstraint($2);
    }
    | inner_expr COMMA inner_expr;

%%

void set_input_string(const char* in);
void end_string_scan(void);


BigResponse parse_request(const char* in) {
  ch = 0;
  logicApi.start();
  response.clear();

  set_input_string(in);
  int res = yyparse();
  end_string_scan();
  response.error.setErrorCode(response.error.getErrorCode() || res);
  return response;
}

int yyerror(const char *errmsg){
    std::string str = std::string(errmsg) + " (Str num " + std::to_string(yylineno) + ", sym num " + std::to_string(ch) +"): "+ std::string(yytext);
    response.error.setErrorMsg(str);
    //fprintf(stderr, "%s (Str num %d, sym num %d): %s\n", errmsg, yylineno, ch, yytext);

    return 0;
}