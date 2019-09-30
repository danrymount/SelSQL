%{
    #include<stdio.h>
    #include<stdlib.h>
    #include <iostream>
    #include <string>

    int yylex();
    int yyerror(char *s);
    int counter = 0;
    std::string parent;
    char* mass;
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
    ddl_actions SEMICOLON {printf("END\n")}| request request;

ddl_actions:
    DDLCREATE table {
        printf("ACTION = %s\n", $1);
    }
    | DDLDROP table{
        printf("ACTION = %s\n", $1);
    }
    | DDLSHOW table{
        printf("ACTION = %s\n", $1);
    };

table:
    TABLE STRING{
        printf("TABLENAME = %s\n", $2);
    }
    |table brackets;

brackets:
    BRACKET inner_expr BRACKET
;

inner_expr:
    STRING TYPE {
	printf("VALNAME = %s\n TYPE = %s\n", $1, $2);
    }
    | inner_expr CONSTRAINT {
    	printf("CONSTRAINT = %s\n", $2);
    }
    | inner_expr COMMA inner_expr;


%%



int yyerror(char *s){
    printf("Syntax Error on line %s\n", s);
    return 0;
}