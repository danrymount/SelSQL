%{
    #define YYERROR_VERBOSE
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <string>
    #include "../../src/Utils/Headers/CommonUtils.h"

    #include "../../src/Parser/Nodes/BaseNode.h"
    #include "../../src/Parser/Nodes/RootNode.h"
    #include "../../src/Parser/Nodes/ConstraintNode.h"
    #include "../../src/Parser/Nodes/VariableNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/CreateNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/DropNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/ShowCreateNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/BaseActionNode.h"


    extern int yylineno;
    extern int ch;
    extern char *yytext;
    int yylex();
    int yyerror(const char *s);

    RootNode *tree;

    std::vector<ConstraintNode*> constraintsList;
    std::vector<VariableNode*> variablesList;
    std::vector<BaseActionNode*> children;

%}

%token CREATE_TABLE_ACTION SHOWCREATE_TABLE_ACTION DROP_TABLE_ACTION INSERT_INTO_ACTION VALUES SELECT_ACTION FROM UPDATE_ACTION SET
%token CONSTR_UNIQUE CONSTR_NOT_NULL CONSTR_PRIMARY_KEY
%token INT FLOAT CHAR
%token IDENT FLOATNUM NUMBER STRVAL LBRACKET RBRACKET SEMICOLON COMMA STAR EQUAL


%type<Constraint> constraint
%type<Variable> variable
%type<t> type
%type<string> IDENT
//%type<string> id
//%type<string> request


%union{
    //char string[256];
    int number;
    char *string;
    ConstraintNode *Constraint;
    VariableNode *Variable;
    Type t;
}

%%
//
//request:
//    id {
//    std::cout << "TT="<< $$ << std::endl;
//    }
//;
//
//id:
//    STRINGG { $$ = $1;}
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
    SELECT_ACTION cols_select FROM IDENT SEMICOLON {

    }|
    UPDATE_ACTION IDENT SET update_list SEMICOLON {

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
    LBRACKET colname RBRACKET

colname:
    IDENT {

    }|
    colname COMMA IDENT {

    }

insert_values:
    values {

    }|
    insert_values COMMA values {

    }

cols_select:
    col_select {

    }|
    cols_select COMMA col_select {

    }

col_select:
    STAR {

    }|
    IDENT {

    }

update_list:
    update_elem {

    }|
    update_list COMMA update_elem {

    }

update_elem:
    IDENT EQUAL values {

    }

values:
    STRVAL {

    }|
    NUMBER {

    }|
    FLOATNUM {

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
    //fprintf(stderr, "%s (Str num %d, sym num %d): %s\n", errmsg, yylineno, ch, yytext);

    return 0;
}