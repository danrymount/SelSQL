%{
    #define YYERROR_VERBOSE
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <ctype.h>
    #include <memory>
    #include "../../src/Utils/Headers/CommonUtils.h"

    #include "../../src/Parser/Nodes/BaseNode.h"
    #include "../../src/Parser/Nodes/RootNode.h"
    #include "../../src/Parser/Nodes/ConstraintNode.h"
    #include "../../src/Parser/Nodes/VariableNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/CreateNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/DropNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/ShowCreateNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/BaseActionNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/InsertNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/SelectNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/DeleteNode.h"
    #include "../../src/Parser/Nodes/ActionNodes/UpdateNode.h"

    #include "../../src/Parser/Nodes/ExpressionsNodes/BaseExprNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/ExprNode.h"

    #include "../../src/Parser/Nodes/ExpressionsNodes/ArithmeticNodes/ArithmeticNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/ArithmeticNodes/AddNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/ArithmeticNodes/DivNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/ArithmeticNodes/MultNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/ArithmeticNodes/SubNode.h"

    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/CmpNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/EqualsNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/LessNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/NoEqualsNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/LessEqNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/MoreNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/CompareNodes/MoreEqNode.h"

    #include "../../src/Parser/Nodes/ExpressionsNodes/LogicNodes/AndLogicNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/LogicNodes/OrLogicNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/LogicNodes/NotLogicNode.h"

    #include "../../src/Parser/Nodes/ExpressionsNodes/IndentNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/UpdateExprNode.h"
    #include "../../src/Parser/Nodes/UpdatesAndExprNode.h"

    #include "../../src/Parser/Nodes/ValuesNodes/BaseValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/IntValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/CharValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/FloatValueNode.h"
    #include "../../src/Parser/Nodes/ValuesNodes/NullValueNode.h"
    #include "../../src/Parser/Nodes/ColumnNode.h"
    #include "../../src/Parser/Nodes/VariableListNode.h"
    #include "../../src/Parser/Nodes/ColumnsAndValuesNode.h"
    #include "../../src/Parser/Nodes/ColumnsAndExprNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/IndentExprNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/ValueExprNode.h"
    #include "../../src/Parser/Nodes/ExpressionsNodes/AssignUpdateNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/SourceJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/BaseJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/JoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/LeftJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/RightJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/FullJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/UnionJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/IntersectJoinNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/UnionIntersectNode.h"
    #include "../../src/Parser/Nodes/TableNode.h"
    #include "../../src/Parser/Nodes/JoinNodes/UnionIntersectListNode.h"

    extern int yylineno;
    extern int ch;
    extern char *yytext;
    std::string* error_msg;
    int yylex();
    int yyerror(const char *s);

    RootNode *tree;

    std::vector<ConstraintNode*> constraintsList;
    std::vector<VariableNode*> variablesList;
    std::vector<BaseActionNode*> children;
    std::vector<BaseValueNode*> valuesList;
    std::vector<ColumnNode*> columnsList;
    std::vector<BaseExprNode*> updateList;
%}

%token CREATE_ACTION SHOW_ACTION DROP_ACTION INSERT_ACTION SELECT_ACTION UPDATE_ACTION DELETE_ACTION TABLE INTO FROM
%token VALUES SET WHERE AS AND OR NOT JOIN LEFT RIGHT FULL ON UNION INTERSECT BEGIN_ COMMIT
%token CONSTR_UNIQUE CONSTR_NOT_NULL CONSTR_PRIMARY_KEY
%token INT_TYPE FLOAT_TYPE CHAR_TYPE
%token IDENT FLOATNUM NUMBER STRVAL VALNULL
%token LBRACKET RBRACKET SEMICOLON COMMA STAR EQUAL NOTEQ PLUS MINUS MORE LESS MOREEQ LESSEQ DIV DOT


%type<Constraint> constraint
%type<Variable> variable
%type<t> type
%type<string> IDENT FLOATNUM NUMBER STRVAL STAR VALNULL
%type<Value> value
%type<Column> colname col_select
%type<Expr> where_exprs where_expr expr_priority_1 expr_priority_2 expr_priority_3 expr_priority_4 expr_priority_5 expr_priority_6 expr update_elem
%type<Cmp> equal_sign
%type<Idt> alias
%type<BaseJoin> join join_type join_expr
%type<UINode> union_intercest
%type<SNode> select
%type<UIList> union_intercest_expr
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
    BaseExprNode* Expr;
    CmpNode* Cmp;
    IdentNode* Idt;
    BaseJoinNode* BaseJoin;
    UnionIntersectNode* UINode;
    SelectNode* SNode;
    UnionIntersectListNode *UIList;

    Type t;
    int charLen;
}

%%
query:
    request {
    	tree = new RootNode(children);

    	variablesList.clear();
    	columnsList.clear();
    	valuesList.clear();
    	children.clear();
    	updateList.clear();

    }|
    transaction {

    }

transaction:
    BEGIN_ BEGIN_ SEMICOLON transaction_request COMMIT SEMICOLON {

    }

transaction_request:
    request {

    }|
    transaction_request request {

    }
request:
    CREATE_ACTION TABLE IDENT LBRACKET variables RBRACKET SEMICOLON{
	children.emplace_back(new CreateNode(new IdentNode(std::string($3)), new VariableListNode(variablesList)));
    }|
    DROP_ACTION TABLE IDENT SEMICOLON{
	children.emplace_back(new DropNode(new IdentNode(std::string($3))));
    }|
    SHOW_ACTION CREATE_ACTION TABLE IDENT SEMICOLON{
	children.emplace_back(new ShowCreateNode(new IdentNode(std::string($4))));
    }|
    INSERT_ACTION INTO IDENT colnames VALUES LBRACKET insert_values RBRACKET SEMICOLON {
	children.emplace_back(new InsertNode(new IdentNode(std::string($3)), new ColumnsAndValuesNode(columnsList, valuesList)));
    }|
    select union_intercest_expr SEMICOLON{
        $2->getChilds()[0]->addChild($1);
    	children.emplace_back($2);
    }|
    select SEMICOLON {
        children.emplace_back($1);
    }
    |
    UPDATE_ACTION IDENT SET update_list where_exprs SEMICOLON {
        children.emplace_back(new UpdateNode(new IdentNode(std::string($2)), new UpdatesAndExprNode(new UpdateExprNode(updateList), new ExprNode($5))));
    }|
    DELETE_ACTION FROM IDENT where_exprs SEMICOLON {
	children.emplace_back(new DeleteNode(new IdentNode(std::string($3)), new ExprNode($4)));
    }

select:
    SELECT_ACTION cols_select FROM IDENT empty where_exprs {
	$$ = new SelectNode(new TableNode(new IdentNode(std::string($4))), new ColumnsAndExprNode(columnsList, new ExprNode($6)));
	columnsList.clear();
    }|
    SELECT_ACTION cols_select FROM join where_exprs {
	$$ = new SelectNode($4, new ColumnsAndExprNode(columnsList, new ExprNode($5)));
	columnsList.clear();
    }

empty:

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
    	if ($2 == Type::TYPE_CHAR) {
    		$$->setSize(yylval.charLen);
    	}
    }|
    IDENT type constraints {
	$$ = new VariableNode(std::string($1), $2, constraintsList);
	if ($2 == Type::TYPE_CHAR) {
		$$->setSize(yylval.charLen);
	}
	constraintsList.clear();
    }

type:
    INT_TYPE {
	$$ = Type::TYPE_INT;
    }|
    FLOAT_TYPE {
	$$ = Type::TYPE_FLOAT;
    }|
    CHAR_TYPE {
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
    /*empty*/ |{
    	columnsList.emplace_back(new ColumnNode(new IdentNode("*")));
    }

colname:
    IDENT {
    	$$ = new ColumnNode(new IdentNode($1));
    }|
    colname COMMA IDENT {
    	columnsList.emplace_back($1);
    	$$ = new ColumnNode(new IdentNode($3));
    }

insert_values:
    value {
	valuesList.emplace_back($1);
    }|
    insert_values COMMA value {
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
    	$$ = new ColumnNode(new IdentNode("*"));
    }|
    IDENT {
    	$$ = new ColumnNode(new IdentNode($1));
    }|
    IDENT DOT IDENT {
	$$ = new ColumnNode(new IdentNode($1), new IdentNode($3));
    }

alias:
    AS IDENT {
	$$ = new IdentNode(std::string($2));
    }|
    /*empty*/ {
	$$ = nullptr;
    }

join:
    join_expr{
    	$$ = $1;
    }|
    join join_type join_expr ON where_expr{
    	$2->addChilds($1, $3, new ExprNode($5));
	$$ = $2;
    }

join_expr:
    IDENT alias{
    	$$ = new SourceJoinNode(new IdentNode($1), $2);
    }|
    LBRACKET join RBRACKET{
	$$ = new SourceJoinNode($2, new IdentNode(""));
    }

join_type:
    JOIN {
	$$ = new JoinNode();
    }|
    LEFT JOIN {
	$$ = new LeftJoinNode();
    }|
    RIGHT JOIN {
	$$ = new RightJoinNode();
    }|
    FULL JOIN {
	$$ = new FullJoinNode();
    }

union_intercest_expr:
    union_intercest {
    	$$ = new UnionIntersectListNode();
    	$$->addUnionIntersectNode($1);
    }|
    union_intercest union_intercest{
    	$$ = new UnionIntersectListNode();
	$$->addUnionIntersectNode($1);
	$$->addUnionIntersectNode($2);
    }

union_intercest:
    UNION select{
	$$ = new UnionJoinNode();
	$$->addChild($2);
    }|
    INTERSECT select{
	$$ = new IntersectJoinNode();
	$$->addChild($2);
     }

update_list:
    update_elem {
	updateList.emplace_back($1);
    }|
    update_list COMMA update_elem {
	updateList.emplace_back($3);
    }

update_elem:
    IDENT EQUAL expr_priority_2 {
	$$ = new AssignUpdateNode(std::string($1), $3);
    }|
    IDENT EQUAL STRVAL {
    	$$ = new AssignUpdateNode(std::string($1), new IdentNode(std::string($3)));
    }|
    IDENT EQUAL VALNULL{
    	$$ = new AssignUpdateNode(std::string($1), new NullValueNode(std::string($1)));
    }

value:
    STRVAL {
	$$ = new CharValueNode(std::string($1));
    }|
    NUMBER {
    try {
    	$$ = new IntValueNode(std::stoi($1));
    } catch(...) {
        yyerror("Int oversize");
        return 1;
    }
    }|
    MINUS NUMBER {
    try {
    	$$ = new IntValueNode(std::stoi("-"+std::string($2)));
    } catch(...) {
        yyerror("Int oversize");
        return 1;
    }
    }|
    FLOATNUM {
    try {
        $$ = new FloatValueNode(std::stod($1));
    } catch(...) {
        yyerror("Float oversize");
        return 1;
    }
    }|
    MINUS FLOATNUM {
    try {
        $$ = new FloatValueNode(std::stod("-"+std::string($2)));
    } catch(...) {
        yyerror("Float oversize");
        return 1;
    }
    }|
    VALNULL {
	$$ = new NullValueNode(std::string($1));
    }

where_exprs:
    WHERE where_expr{
    	$$ = $2;
    }|
    /*empty*/ { $$ = new ExprNode();};

where_expr:
    expr_priority_6{
    	$$ = $1;
    }|
    where_expr OR expr_priority_6 {
	$$ = new OrLogicNode($1, $3);
    }


expr_priority_6:
    expr_priority_5{
    	$$ = $1;
    }|
    expr_priority_6 AND expr_priority_5{
	$$ = new AndLogicNode($1, $3);
    }

expr_priority_5:
    expr_priority_4 {
    	$$ = $1;
    }|
    NOT expr_priority_5 {
    	$$ = new NotLogicNode($2);
    }

expr_priority_4:
    expr_priority_3 {
    	$$ = $1;
    }|
    LBRACKET where_expr RBRACKET{
    	$$ = $2;
    }

expr_priority_3:
    expr_priority_2{
    	$$ = $1;
    }|
    expr_priority_3 equal_sign expr_priority_2{
    	$2->addChilds($1, $3);
    	$$ = $2;
    }|
    expr_priority_3 equal_sign STRVAL{
        $2->addChilds($1, new IdentNode(std::string($3)));
        $$ = $2;
    }

expr_priority_2:
    expr_priority_1{
    	$$ = $1;
    }|
    expr_priority_2 PLUS expr_priority_1{
	$$ = new AddNode($1, $3);
    }|
    expr_priority_2 MINUS expr_priority_1{
    	$$ = new SubNode($1, $3);
    }

expr_priority_1:
    expr{
    	$$ = $1;
    }|
    expr_priority_1 STAR expr{
	$$ = new MultNode($1, $3);
    }|
    expr_priority_1 DIV expr{
    	$$ = new DivNode($1, $3);
    }

expr:
    NUMBER {
	$$ = new ValueExprNode(std::string($1));
    }|
    FLOATNUM {
	$$ = new ValueExprNode(std::string($1));
    }|
    MINUS NUMBER {
	$$ = new ValueExprNode("-" + std::string($2));
    }|
    MINUS FLOATNUM {
	$$ = new ValueExprNode("-" + std::string($2));
    }|
    IDENT {
	$$ = new IndentExprNode(std::string($1));
    }|
    IDENT DOT IDENT {
	$$ = new IndentExprNode(std::string($1),std::string($3));
    }|
    LBRACKET expr_priority_2 RBRACKET {
	$$ = $2;
    }

equal_sign:
    EQUAL {
	$$ = new EqualsNode();
    }|
    NOTEQ {
	$$ = new NoEqualsNode();
    }|
    MORE {
	$$ = new MoreNode();
    }|
    LESS {
	$$ = new LessNode();
    }|
    MOREEQ {
	$$ = new MoreEqNode();
    }|
    LESSEQ {
	$$ = new LessEqNode();
    }
%%

void set_input_string(const char* in);
void end_string_scan(void);


RootNode * parse_request(const char* in, std::string* msg) {
  ch = 0;
  error_msg = msg;
  set_input_string(in);
  int res = yyparse();
  end_string_scan();
  return tree;
}

int yyerror(const char *errmsg){


    std::string str = std::string(errmsg) + " (Str num " + std::to_string(yylineno) + ", sym num " + std::to_string(ch) +"): "+ std::string(yytext);
    *error_msg = str;
//    fprintf(stderr, "%s (Str num %d, sym num %d): %s\n", errmsg, yylineno, ch, yytext);

    variablesList.clear();
    columnsList.clear();
    valuesList.clear();
    children.clear();
    updateList.clear();
    tree = nullptr;

    return 0;
}