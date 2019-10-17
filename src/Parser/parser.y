%{
    #define YYERROR_VERBOSE
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <string>

    extern int yylineno;
    extern int ch;
    extern char *yytext;
    int yylex();
    int yyerror(const char *s);
%}

%token <string> STRINGG

%type<string> id
%type<string> request


%union{
    //char string[256];
    int number;
    char *string;
}

%%

request:
    id {
    std::cout << "TT="<< $$ << std::endl;
    }
;

id:
    STRINGG { $$ = $1;}

%%

void set_input_string(const char* in);
void end_string_scan(void);


int parse_request(const char* in) {
  ch = 0;

  set_input_string(in);
  int res = yyparse();
  end_string_scan();
  return res;
}

int yyerror(const char *errmsg){
    std::string str = std::string(errmsg) + " (Str num " + std::to_string(yylineno) + ", sym num " + std::to_string(ch) +"): "+ std::string(yytext);
    //fprintf(stderr, "%s (Str num %d, sym num %d): %s\n", errmsg, yylineno, ch, yytext);

    return 0;
}