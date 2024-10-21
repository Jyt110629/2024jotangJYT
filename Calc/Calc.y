%{
#include <stdio.h>
void yyerror(const char* msg) {}
%}

%token T_NUM
%token INC BOM BAM

%left '+' '-'
%left '*' '/'
%right INC
%left BAM BOM

%%

S   :   S E '\n'        { printf("ans = %d\n", $2); }
    |   /* empty */     { /* empty */ }
    ;

E   :   E '+' E         { $$ = $1 + $3; }
    |   E '-' E         { $$ = $1 - $3; }
    |   E '*' E         { $$ = $1 * $3; }
    |   E '/' E         { $$ = $1 / $3; }
    |   T_NUM           { $$ = $1; }
    |   '(' E ')'       { $$ = $2; }
    |	E INC E		{ $$ = $1 + ($3+1);}
    |	E BAM E		{ $$ = ($1 & $3) * $3;}
    |	E BOM E		{ $$ = ($1 | $3) * $3;}
    ;

%%

int main() {
    printf("Enter expressions:\n");
    return yyparse();
}