%option yylineno
%pointer

%{
    #include <cstdlib>
    #include <cerrno>
    #include <climits>
    #include <limits>
    #include "parser.hpp"
    
	void yyerror(const char *);
%}


%x COMMENT

UNSIGN_INT             0|([1-9]+[0-9]*)
IDENTIFIER             [A-z][A-z0-9]*


%%


"+"               { return '+'; }
"-"               { return '-'; }
"*"               { return '*'; }
"/"               { return '/'; }
"("               { return '('; }
")"               { return ')'; }
"{"               { return '{'; }
"}"               { return '}'; }
";"               { return ';'; }
","               { return ','; }
"."               { return '.'; }
"="               { return '='; }
">"               { return _GREATER; }
">="              { return _GREATEREQ; }
"->"              { return _ARROW; }
"equals"          { return _EQUALS; }
"and"             { return _AND; }
"or"              { return _OR; }
"not"             { return _NOT; }
"new"             { return _NEW_; }
"true"            { return _TRUE; }
"false"           { return _FALSE; }
"if"              { return _IF; }
"else"            { return _ELSE; }
"while"           { return _WHILE; }
"do"              { return _DO; }
"print"           { return _PRINT; }
"return"          { return _RETURN; }
"extends"         { return _EXTENDS; }
"integer"         { return _INTEGER_LITERAL; }
"boolean"         { return _BOOLEAN_LITERAL; }
"none"            { return _NONE; }

{UNSIGN_INT}      { return _INT; }
{IDENTIFIER}      { return _ID; }

    /* ignore white */
" "               { /* ignore space*/ }
"\n"              { /* ignore newline*/ }
"\t"              { /* ignore tab*/ }

    /* comments */
"/*"              { BEGIN(COMMENT); }
<COMMENT>\n       {  }
<COMMENT>.        {  }
<COMMENT><<EOF>>  { yyerror("EOF"); }
<COMMENT>"*/"     { BEGIN(INITIAL); }

.                 { yyerror("invalid character"); }

%%

int yywrap(void) {
  return 1;
}
