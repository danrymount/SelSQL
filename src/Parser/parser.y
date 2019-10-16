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

%token <string> STRING OTHER SEMICOLON COMMA DDLCREATE DDLSHOW DDLDROP BRACKET TYPE CONSTRAINT DMLINSERT VALUES COMP
NUMBER WHERE EQUALLY FROM DQLSELECT DMLDELETE DMLUPDATE SET ALL VALNULL FLOATNUM SIGN VALSTR LOGIC NOT NOTEQUALLY DIV



%union{
    char string[256];
}

%%

request:
    actions SEMICOLON {
    	response = logicApi.finish();
    	printf("FINISH\n");
    }
    |
    request request
;

actions:
    DDLCREATE STRING inner_expr {
        logicApi.addActionName($1);
        logicApi.addTableName($2);
    }
    |
    DDLDROP STRING{
        logicApi.addActionName($1);
        logicApi.addTableName($2);
    }
    |
    DDLSHOW STRING{
        logicApi.addActionName($1);
        logicApi.addTableName($2);
    }
    |
    DMLINSERT insert_where {
    	logicApi.addActionName($1);
	printf("INSERT\n");
    }
    |
    table_select {
    	logicApi.addActionName("select");

        printf("SELECT\n");
    }
    |
    table_delete {
    	logicApi.addActionName("delete");
        printf("DELETE\n");
    }
    |
    DMLUPDATE STRING table_update {
    	logicApi.addActionName($1);
    	logicApi.addTableName($2);
        printf("%s, %s\n", $1, $2);
    }

table_update:
    update_set
    |
    table_update WHERE where

update_set:
    SET STRING EQUALLY VALSTR {
    	logicApi.addColumn($2);
    	logicApi.addValue($4);
        printf("UPCOL = %s, UPVALSTR = %s\n", $2, $4);
    }
    |
    SET STRING EQUALLY NUMBER {
    	logicApi.addColumn($2);
	logicApi.addValue($4);
        printf("UPCOL = %s, UPVALNUM = %s\n", $2, $4);
    }
    |
    SET STRING EQUALLY VALNULL {
    	logicApi.addColumn($2);
        logicApi.addValue($4);
        printf("UPCOL = %s, UPVALNULL = %s\n", $2, $4);
    }
    |
    SET STRING EQUALLY FLOATNUM {
    	logicApi.addColumn($2);
    	logicApi.addValue($4);
        printf("UPCOL = %s, UPVALFLOAT = %s\n", $2, $4);
    }
    |
    update_set COMMA STRING EQUALLY VALSTR {
    	logicApi.addColumn($3);
    	logicApi.addValue($5);
    	printf("UPCOL = %s, UPVALSTR = %s\n", $3, $5);
    }
    |
    update_set COMMA STRING EQUALLY NUMBER {
   	logicApi.addColumn($3);
    	logicApi.addValue($5);
        printf("UPCOL = %s, UPVALNUM = %s\n", $3, $5);
    }
    |
    update_set COMMA STRING EQUALLY VALNULL {
    	logicApi.addColumn($3);
    	logicApi.addValue($5);
        printf("UPCOL = %s, UPVALNULL = %s\n", $3, $5);
    }
    |
    update_set COMMA STRING EQUALLY FLOATNUM {
    	logicApi.addColumn($3);
    	logicApi.addValue($5);
        printf("UPCOL = %s, UPVALFLOAT = %s\n", $3, $5);
    }

table_delete:
    DMLDELETE STRING {
    	logicApi.addTableName($2);
        printf("TABLE = %s\n", $2);
    }
    |
    DMLDELETE STRING WHERE where {
    	logicApi.addTableName($2);
        printf("TABLE = %s\n", $2);
    }

table_select:
    col_select FROM STRING {
    	logicApi.addTableName($3);
        printf("TABLE = %s\n", $3);
    }
    |
    table_select WHERE where

col_select:
    DQLSELECT STRING {
    	logicApi.addSelectColumn($2);
    	printf("COL = %s\n", $2);
    }
    |
    DQLSELECT ALL {
    	logicApi.addSelectColumn($2);
        printf("COLALL = %s\n", $2);
    }
    |
    col_select COMMA STRING {
    	logicApi.addSelectColumn($3);
    	printf("COL = %s\n", $3);
    }
    |
    col_select COMMA ALL {
    	logicApi.addSelectColumn($3);
        printf("COLALL = %s\n", $3);
    }

insert_where:
    insert

where:
    NOT where {
    	logicApi.expression.addLogicOperator($1);
    	printf("NOT\n");
    }|
    where LOGIC where {
    	logicApi.expression.addLogicOperator($2);
    	printf("%s\n", $2);
    }|
    BRACKET where BRACKET {
    	logicApi.expression.addOperator($1);
    	logicApi.expression.addOperator($3);
    	printf("%s %s\n", $1, $3);
    }|
    STRING EQUALLY expr2 {
    	logicApi.expression.addColumn($1, $2);
    	printf("%s = \n", $1);
    }|
    STRING NOTEQUALLY expr {
    	logicApi.expression.addColumn($1, $2);
    	printf("%s != \n", $1);
    }|
    STRING EQUALLY VALSTR {
    	logicApi.expression.addOperand($3);
    	logicApi.expression.addColumn($1, $2);
    	printf("%s = %s\n", $1, $3);
    }|
    STRING NOTEQUALLY VALSTR {
    	logicApi.expression.addOperand($3);
    	logicApi.expression.addColumn($1, $2);
    	printf("%s != %s\n", $1, $3);
    }|
    STRING COMP expr {
    	logicApi.expression.addColumn($1, $2);
    	printf("%s %s \n", $1, $2);
    }

expr31:
    BRACKET expr2 BRACKET

expr2:
    expr1|
    expr2 SIGN expr1 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr31 SIGN expr2 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr2 SIGN expr31 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr31 SIGN expr31 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }

expr1:
    expr|
    expr1 ALL expr {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }|
    expr1 DIV expr {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr31 DIV expr {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr31 ALL expr {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr1 DIV expr31 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr1 ALL expr31 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr31 DIV expr31 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }
    |
    expr31 ALL expr31 {
    	logicApi.expression.addOperand($2);
    	printf("%S", $2);
    }

expr:
    STRING {
    	logicApi.expression.addOperand($1);
    	printf("%s\n", $1);
    }
    |
    NUMBER {
    	logicApi.expression.addOperand($1);
        printf("%s \n", $1);
    }
    |
    FLOATNUM {
    	logicApi.expression.addOperand($1);
        printf("%s\n", $1);
    }

//where:
//    WHERE STRING EQUALLY VALSTR {
//    	logicApi.addCondition($2, $3, $4);
//        printf("WHERE %s %s %s\n", $2, $3, $4);
//    }
//    |
//    WHERE STRING EQUALLY NUMBER {
//    	logicApi.addCondition($2, $3, $4);
//        printf("WHERE %s %s %s\n", $2, $3, $4);
//    }
//    |
//    WHERE STRING EQUALLY FLOATNUM {
//    	logicApi.addCondition($2, $3, $4);
//        printf("WHERE %s %s %s\n", $2, $3, $4);
//    }
//    |
//    WHERE STRING EQUALLY VALNULL {
//    	logicApi.addCondition($2, $3, $4);
//        printf("WHERE %s %s %s\n", $2, $3, $4);
//    }

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

values:
    VALUES BRACKET
    |
    values VALSTR {
    	logicApi.addValue($2);
    	std::cout << $2 << std::endl;
    	printf("VALSTR = %s\n", $2);
    }
    |
    values NUMBER {
    	logicApi.addValue($2);
    	printf("VALNUM = %s\n", $2);
    }
    |
    values VALNULL {
    	logicApi.addValue($2);
    	printf("VALNULL = %s\n", $2);
    }
    |
    values FLOATNUM {
    	logicApi.addValue($2);
    	printf("VALFLOAT = %s\n", $2);
    }
    |
    values COMMA VALSTR {
    	logicApi.addValue($3);
         printf("VALSTR = %s\n", $3);
    }
    |
    values COMMA NUMBER {
    	logicApi.addValue($3);
        printf("VALNUM = %s\n", $3);
    }
    |
    values COMMA VALNULL {
    	logicApi.addValue($3);
        printf("VALNULL = %s\n", $3);
    }
    |
    values COMMA FLOATNUM {
    	logicApi.addValue($3);
        printf("VALFLOAT = %s\n", $3);
    }
    |
    values BRACKET

inner_expr:
    BRACKET STRING TYPE {
	logicApi.addColumn($2, $3);
    }
    | inner_expr CONSTRAINT {
    	logicApi.addConstraint($2);
    }
    | inner_expr COMMA STRING TYPE {
	logicApi.addColumn($3, $4);
    }
    | inner_expr BRACKET;

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