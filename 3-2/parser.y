%{
    #include <cstdlib>
    #include <cstdio>
    #include <iostream>

    #define YYDEBUG 1

    int yylex(void);
    void yyerror(const char *);
%}

%error-verbose

/* WRITEME: List all your tokens here */
%token _INT _ID _TRUE _FALSE _NONE
%token _NEW_ // _NEW seems to be a reserved word in C (which is a bug in Bison)
%token ',' '='
%token _IF _ELSE _WHILE _DO _PRINT _ARROW _RETURN _EXTENDS
%token _INTEGER_LITERAL _BOOLEAN_LITERAL
%precedence U_MINUS


/* WRITEME: Specify precedence here */
%left '*' '/'
%left '+' '-'
%left _GREATER _GREATEREQ _EQUALS
%left _AND _OR
%precedence _NOT

%%

/* WRITEME: This rule is a placeholder, since Bison requires
            at least one rule to run successfully. Replace
            this with your appropriate start rules. */
Start : Classes
      ;

Classes : %empty
        | Class Classes
        ;

Class : ClassName _EXTENDS ClassName '{' MembersAndMethods '}' 
      | ClassName '{' MembersAndMethods '}' 
      ;

// can be %empty
MembersAndMethods: Member MembersAndMethods  // pop member
      | Methods // methods now
      ;

Member: Type _ID ';'
      ;


Methods : Method Methods
      | %empty
      ;

Method :  _ID '(' Parameters ')' _ARROW Type '{' DeclarationsAndStatementsAndReturn '}'
      ;
     
      /* abbr: non-produceless self derivation, seperators-1 */
Parameters : %empty
           | Type _ID ParametersRest
           ;

ParametersRest : %empty
               | ',' Type _ID ParametersRest
               ;

// Both Declaration and Statements are list and can be %empty, 
// Thus explicitly make them perform as states

DeclarationsAndStatementsAndReturn: Declaration DeclarationsAndStatementsAndReturn
      | StatementsAndReturn
      ;

Declaration: Type IdsNonempty ';' 
      ;

IdsNonempty : _ID
            | _ID ',' IdsNonempty
            ;

// statements in Methods; thus add return statement here
StatementsAndReturn : Statement StatementsAndReturn
           | Return
           ;

Return: _RETURN Exp ';'
      | %empty


Statement   : _ID '=' Exp ';'
            | _ID '.' _ID '=' Exp ';'
            | MethodCall ';'
            | _IF Exp '{' Block '}' 
            | _IF Exp '{' Block '}' _ELSE '{' Block '}'
            | _WHILE Exp '{' Block '}'
            | _DO '{' Block '}' _WHILE '(' Exp ')' ';'
            | _PRINT Exp ';'
            ;

Block : Statement
      | Statement Block
      ;

Exp   : Exp '+' Exp
      | Exp '-' Exp
      | Exp '*' Exp
      | Exp '/' Exp
      | Exp _GREATER Exp
      | Exp _GREATEREQ Exp
      | Exp _EQUALS Exp      
      | Exp _AND Exp
      | Exp _OR Exp
      | _NOT Exp
      | '-' Exp %prec U_MINUS
      | _ID
      | _ID '.' _ID
      | MethodCall
      | '(' Exp ')'
      | _INT
      | _TRUE
      | _FALSE
      | _NEW_ ClassName
      | _NEW_ ClassName '(' Args ')'
      ;

MethodCall  : _ID '(' Args ')'
            | _ID '.' _ID '(' Args ')'
            ;

Args  : ArgsRest
      | %empty
      ;

ArgsRest  : ArgsRest ',' Exp
          | Exp
          ;

Type  : _INTEGER_LITERAL
      | _BOOLEAN_LITERAL
      | _NONE            
      | _ID
      ;

// MethodName : _ID
//            ;

ClassName : _ID
          ;


/* WRITME: Write your Bison grammar specification here */






%%

extern int yylineno;

void yyerror(const char *s) {
  fprintf(stderr, "%s at line %d\n", s, yylineno);
  exit(1);
}
