%{
    #include <stdio.h>
    #include <string>
    #include "parser.h"
    #include <memory>
    #include <cstring>
    void showError();
    int ch;
%}

%option noinput noyywrap case-insensitive yylineno


words   ([A-Za-z0-9]*)

%%

{words}         { ch += yyleng; yylval.string = new char[100](); sscanf(yytext, "%s", yylval.string); return STRINGG; }

%%

void showError(){
    printf("Other input");
}

void set_input_string(const char* in) {
  yy_scan_string(in);
}

void end_string_scan(void) {
  yy_delete_buffer(YY_CURRENT_BUFFER);
}