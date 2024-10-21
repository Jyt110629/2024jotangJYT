%{
#include "y.tab.h"
%}

%%
[0-9]+          { yylval = atoi(yytext); return T_NUM; }
[-/+*()\n]      { return yytext[0]; }
" "		{  /* end when meet everything else */ }
.               {  /* end when meet everything else */ }
"++"		{ return INC;}
"&*"		{ return BAM;}
"|*"		{ return BOM;}
%%

int yywrap(void) { 
    return 1;
}