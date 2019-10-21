%{
    #define YYERROR_VERBOSE
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <ctype.h>
    #include "../../src/Utils/Headers/CommonUtils.h"

    #include "../../src/Parser/Nodes/BaseNode.h"
    #include "../../src/Parser/Nodes/RootNode.h"
    #include "../../src/Parser/Nodes/ConstraintNode.h"
    #include "../../src/Parser/Nodes/VariableNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/CreateNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/DropNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/ShowCreateNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/BaseActionNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/BaseValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/IntValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/CharValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/FloatValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/NullValueNode.h"
    #include "../../src/Parser/Nodes/ColumnNode.h"


    extern int yylineno;
    extern int ch;
    extern char *yytext;
    int yylex();
    int yyerror(const char *s);

    RootNode *tree;

    std::vector<ConstraintNode*> constraintsList;
    std::vector<VariableNode*> variablesList;
    std::vector<BaseActionNode*> children;
    std::vector<BaseValueNode*> valuesList;
    std::vector<ColumnNode*> columnsList;
%}

%token CREATE_TABLE_ACTION SHOWCREATE_TABLE_ACTION DROP_TABLE_ACTION INSERT_INTO_ACTION SELECT_ACTION UPDATE_ACTION DELETE_FROM_ACTION
%token VALUES FROM SET WHERE AND OR NOT
%token CONSTR_UNIQUE CONSTR_NOT_NULL CONSTR_PRIMARY_KEY
%token INT FLOAT CHAR
%token IDENT FLOATNUM NUMBER STRVAL VALNULL
%token LBRACKET RBRACKET SEMICOLON COMMA STAR EQUAL NOTEQ PLUS MINUS MORE LESS MOREEQ LESSEQ DIV


%type<Constraint> constraint
%type<Variable> variable
%type<t> type
%type<string> IDENT FLOATNUM NUMBER STRVAL STAR
%type<Value> values
%type<Column> colname col_select
//%type<string> id
//%type<string> request


%union{
    //char string[256];
    int number;
    char *string;
    ConstraintNode *Constraint;
    VariableNode *Variable;
    BaseValueNode* Value;
    ColumnNode* Column;
    Type t;
    int charLen;
}

%%
query:
    request {
    	tree = new RootNode(children);

    	variablesList.clear();
    	children.clear();
    }

request:
    CREATE_TABLE_ACTION IDENT LBRACKET variables RBRACKET SEMICOLON{
    	children.emplace_back(new CreateNode(std::string($2), variablesList));
    }|
    DROP_TABLE_ACTION IDENT SEMICOLON{
	children.emplace_back(new DropNode(std::string($2), variablesList));
    }|
    SHOWCREATE_TABLE_ACTION IDENT SEMICOLON{
	children.emplace_back(new ShowCreateNode(std::string($2)));
    }|
    INSERT_INTO_ACTION IDENT colnames VALUES insert_values SEMICOLON {

    }|
    SELECT_ACTION cols_select FROM IDENT where_exprs SEMICOLON {

    }|
    UPDATE_ACTION IDENT SET update_list where_exprs SEMICOLON {

    }|
    DELETE_FROM_ACTION IDENT where_exprs SEMICOLON {

    }

variables:
    variable {
	variablesList.emplace_back($1);
    }|
    variables COMMA variable {
    	variablesList.emplace_back($3);
    }

variable:
    IDENT type{
    	$$ = new VariableNode(std::string($1), $2);
    }|
    IDENT type constraints {
	$$ = new VariableNode(std::string($1), $2, constraintsList);
	constraintsList.clear();
    }

type:
    INT {
	$$ = Type::TYPE_INT;
    }|
    FLOAT {
	$$ = Type::TYPE_FLOAT;
    }|
    CHAR {
	$$ = Type::TYPE_CHAR;
    }

constraints:
    constraint {
	constraintsList.emplace_back($1);
    }|
    constraints constraint{
    	constraintsList.emplace_back($2);
    }

constraint:
    CONSTR_UNIQUE {
    	$$ = new ConstraintNode(Constraint::UNIQUE);
    }|
    CONSTR_NOT_NULL {
	$$ = new ConstraintNode(Constraint::NOT_NULL);
    }|
    CONSTR_PRIMARY_KEY {
	$$ = new ConstraintNode(Constraint::PRIMARY_KEY);
    }

colnames:
    LBRACKET colname RBRACKET{
	columnsList.emplace_back($2);
    }

colname:
    IDENT {
	$$ = new ColumnNode(std::string($1));
    }|
    colname COMMA IDENT {
	$$ = new ColumnNode(std::string($3));
    }

insert_values:
    values {
	valuesList.emplace_back($1);
    }|
    insert_values COMMA values {
	valuesList.emplace_back($3);
    }

cols_select:
    col_select {
	columnsList.emplace_back($1);
    }|
    cols_select COMMA col_select {
	columnsList.emplace_back($3);
    }

col_select:
    STAR {
	$$ = new ColumnNode($1);
    }|
    IDENT {
	$$ = new ColumnNode($1);
    }

update_list:
    update_elem {

    }|
    update_list COMMA update_elem {

    }

update_elem:
    IDENT EQUAL expr_priority_2 {

    }|
    IDENT EQUAL STRVAL {

    }

values:
    STRVAL {
	$$ = new CharValueNode(std::string($1), 0);
    }|
    NUMBER {
	$$ = new IntValueNode(std::stoi($1));
    }|
    FLOATNUM {
	$$ = new FloatValueNode(std::stod($1));
    }|
    VALNULL {

    }

where_exprs:
    WHERE where_expr|
    /*empty*/ { };

where_expr:
    expr_priority_4|
    not where_expr {

    }|
    where_expr logic_or where_expr {

    }|
    LBRACKET where_expr RBRACKET

expr_priority_4:
    expr_priority_3|
    expr_priority_4 logic_and expr_priority_4|
    LBRACKET expr_priority_4 RBRACKET


expr_priority_3:
    expr_priority_2|
    expr_priority_3 equal_sign expr_priority_2|
    expr_priority_3 equal_sign strval

expr_priority_2:
    expr_priority_1|
    expr_priority_2 sign_priority_2 expr_priority_1

expr_priority_1:
    expr|
    expr_priority_1 sign_priority_1 expr

expr:
    NUMBER {

    }|
    FLOATNUM {

    }|
    IDENT {

    }|
    LBRACKET expr_priority_2 RBRACKET {

    }

strval:
    STRVAL {

    }

not:
    NOT {

    }

logic_and:
    AND {

    }

logic_or:
    OR {

    }

equal_sign:
    EQUAL {

    }|
    NOTEQ {

    }|
    MORE {

    }|
    LESS {

    }|
    MOREEQ {

    }|
    LESSEQ {

    }

sign_priority_2:
    PLUS {

    }|
    MINUS {

    }

sign_priority_1:
    STAR {

    }|
    DIV {

    }
%%

void set_input_string(const char* in);
void end_string_scan(void);


RootNode * parse_request(const char* in) {
  ch = 0;

  set_input_string(in);
  int res = yyparse();
  end_string_scan();
  return tree;
}

int yyerror(const char *errmsg){
    std::string str = std::string(errmsg) + " (Str num " + std::to_string(yylineno) + ", sym num " + std::to_string(ch) +"): "+ std::string(yytext);
    fprintf(stderr, "%s (Str num %d, sym num %d): %s\n", errmsg, yylineno, ch, yytext);

    return 0;
}