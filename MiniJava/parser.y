%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
#include "TokenPrinter.h"
extern "C" int yylex();
using std::make_shared;
using std::shared_ptr;

// void yyerror( std::shared_ptr<CProgram>&, int*, const char *);
%}

/*__________ The Bison Declarations Section __________*/

/*This is the best place to write dependency code required for YYSTYPE and YYLTYPE. 
In other words, it’s the best place to define types referenced in %union directives.*/
%code requires { #include <common.h> }
%code requires { #include <memory> }

%parse-param { std::shared_ptr<CProgram>& root }
%parse-param { int* hasError }

// %error-verbose

%union{
    int                     ival;
    bool                    bval;
    char                    sval[255];
    CProgram*               program;
    CMainClass*             mainClass;
    CClassDeclarationList*  classDecls;
    CClassDeclaration*      classDecl;
    CStatementList*         statements;
    IStatement*             statement;
    CVarDeclarationList*    varDecls;
    CVarDeclaration*        varDecl;
    CMethodDeclarationList* methodDecls;
    CMethodDeclaration*     methodDecl;
    CType*                  type;
    CAccessModifier*        accessMod;
    CMethodArguments*       methodArgs;
    CExpressionList*        expList;
    IExpression*            exp;
}

%left '&'
%left '<' '='
%left '+' '-'
%left '*' '/'
%left UMINUS '!'
%left '.' '[' ']'

%token CLASS
%token PUBLIC
%token STATIC
%token VOID
%token MAIN
%token EXTENDS
%token RETURN
%token ELSE
%token IF
%token WHILE
%token SYSTEM_OUT_PRINTLN
%token LENGTH
%token NEW
%token THIS
%token <sval> ID
%token <ival> INTEGER_LITERAL
%token <bval> LOGIC_LITERAL
%token STRING
%token BOOLEAN
%token INT

%type <program>     Program
%type <mainClass>   MainClass
%type <classDecls>  ClassDeclarations
%type <classDecl>   ClassDeclaration
%type <statements>  Statements
%type <statement>   Statement
%type <varDecls>    VarDeclarations
%type <varDecl>     VarDeclaration
%type <methodDecls> MethodDeclarations
%type <methodDecl>  MethodDeclaration
%type <type>        Type
%type <accessMod>   AccessModifier
%type <methodArgs>  MethodArguments
%type <expList>     Expressions
%type <exp>         Expression

%%
/*__________ The Grammar Rules Section __________*/

Program:
      MainClass ClassDeclarations { program = new CProgram(  ) }
    ;

MainClass:
      CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statement '}' '}'
      { $$ }
    ;

ClassDeclarations:
      %empty
    | ClassDeclarations ClassDeclaration
    ;

ClassDeclaration:
      CLASS ID '{' VarDeclarations MethodDeclarations '}'
    | CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'
    ;

VarDeclarations:
      %empty
    | VarDeclarations VarDeclaration
    ;

VarDeclaration:
      Type ID ';'
    ;

MethodDeclarations:
      %empty
    | MethodDeclarations MethodDeclaration
    ;

MethodDeclaration:
      AccessModifier Type ID '(' MethodArguments ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
    ;

Type:
      INT '['']'
    | BOOLEAN
    | INT
    | ID
    ;

AccessModifier:
      PUBLIC
    | PRIVATE
    ;

MethodArguments:
      %empty
    | NonEmptyMethodArguments
    ;

MethodArgumentsNonEmpty:
      MethodArgument
    | NonEmptyMethodArguments ',' MethodArgument
    ;

MethodArgument:
      Type ID
    ;

Statements:
      %empty
    | Statement Statements
    ;

Statement:
      '{' Statements '}'
    | 'if' '(' Expression ')' Statement ELSE Statement
    | WHILE '(' Expression ')' Statement
    | SOUT '(' Expression ')' ';'
    | ID '=' Expression ';'
    | ID '[' Expression ']' '=' Expression ';'
    ;

Expressions:
      %empty              { $$ = new CExpressionList() }
    | ExpressionsNonEmpty { $$ = $1 }
    ;

ExpressionsNonEmpty:
      Expression                         { $$ = new CExpressionList( $1 ) }
    | ExpressionsNonEmpty ',' Expression { $$ = $1; $$.Add( $3 ) }
    ;

Expression:
      Expression "&&" Expression { $$ = new CBinaryExpression( TOperandType::OT_And,   $1, $3 ) }
    | Expression "||" Expression { $$ = new CBinaryExpression( TOperandType::OT_Or,    $1, $3 ) }
    | Expression '<' Expression  { $$ = new CBinaryExpression( TOperandType::OT_LT,    $1, $3 ) }
    | Expression '+' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Plus,  $1, $3 ) }
    | Expression '-' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Minus, $1, $3 ) }
    | Expression '*' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Times, $1, $3 ) }
    | Expression '/' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Div,   $1, $3 ) }
    | Expression '%' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Mod,   $1, $3 ) }

    | Expression '[' Expression ']'         { $$ = new CBracketExpression( $1, $3 )    }
    | Expression '.' LENGTH                 { $$ = new CLengthExpression( $1 )         }
    | Expression '.' ID '(' Expressions ')' { $$ = new CMethodExpression( $1, $3, $5 ) }

    | INTEGER_LITERAL            { $$ = new CNumberExpression( $1 )   }
    | LOGIC_LITERAL              { $$ = new CLogicExpression( $1 )    }
    | ID                         { $$ = new CIdExpression( $1 )       }
    | THIS                       { $$ = new CThisExpression()         }
    | NEW INT '[' Expression ']' { $$ = new CNewArrayExpression( $4 ) }
    | NEW ID '(' ')'             { $$ = new CNewIdExpression( $2 )    }
    | '!' Expression             { $$ = new CNegateExpression( $2)    }
    | '(' Expression ')'         { $$ = $1                            }
    ;
%%
