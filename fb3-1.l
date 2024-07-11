%{
#include <stdio.h>
#include <stdlib.h>
#include "fb3-1.tab.h"
%}

%%
[0-9]+                { yylval.d = atof(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.s = strdup(yytext); return IDENTIFIER; }
"+"                   { return ADD; }
"-"                   { return SUB; }
"*"                   { return MUL; }
"/"                   { return DIV; }
"="                   { return ASSIGN; }
"("                   { return OP; }
")"                   { return CP; }
\n                    { return EOL; }
[ \t]                 { /* skip whitespace */ }
.                     { return yytext[0]; }
%%

int yywrap() {
    return 1;
}

