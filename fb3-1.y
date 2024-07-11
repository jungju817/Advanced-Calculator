%{
#include <stdio.h>
#include <stdlib.h>
#include "fb3-1.h"

void yyerror(const char *s);
%}

%union {
    double d;
    struct ast *a;
    char *s;
}

%token <d> NUMBER
%token <s> IDENTIFIER
%token ADD SUB MUL DIV ASSIGN OP CP EOL

%type <a> exp term factor

%%
input:
      | input line
      ;

line: exp EOL { printf("= %4.4g\n", eval($1)); treefree($1); }
    | IDENTIFIER ASSIGN exp EOL { eval(newasgn($1, $3)); printf("%s = %4.4g\n", $1, eval($3)); }
    ;

exp: term
    | exp ADD term { $$ = newast('+', $1, $3); }
    | exp SUB term { $$ = newast('-', $1, $3); }
    ;

term: factor
    | term MUL factor { $$ = newast('*', $1, $3); }
    | term DIV factor { $$ = newast('/', $1, $3); }
    ;

factor: NUMBER { $$ = newnum($1); }
      | IDENTIFIER { $$ = newref($1); }
      | OP exp CP { $$ = $2; }
      ;

%%
int main(void) {
    printf("Enter expressions:\n");
    return yyparse();
}

