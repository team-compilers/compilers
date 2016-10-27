%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
extern "C" int yylex();
// void yyerror( std::shared_ptr<CProgram>&, int*, const char *);
%}

/*__________ The Bison Declarations Section __________*/

/*This is the best place to write dependency code required for YYSTYPE and YYLTYPE. 
In other words, it’s the best place to define types referenced in %union directives.*/
%code requires {
    #include <memory>
    #include <Program.h>
    #include <MainClass.h>
    #include <ClassDeclarationList.h>
    #include <ClassDeclaration.h>
    #include <Statement.h>
    #include <StatementList.h>
    #include <VarDeclaration.h>
    #include <VarDeclarationList.h>
    #include <MethodDeclaration.h>
    #include <MethodDeclarationList.h>
    #include <TypeModifier.h>
    #include <AccessModifier.h>
    #include <MethodArgument.h>
    #include <MethodArgumentList.h>
    #include <ExpressionList.h>
    #include <Expression.h>
}

%parse-param { std::unique_ptr<CProgram>& root }
%parse-param { int* hasError }

%error-verbose

%union {
    int                     ival;
    bool                    bval;
    char*                   sval;
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
    ITypeModifier*          type;
    IAccessModifier*        accessMod;
    CMethodArgument*        methodArg;
    CMethodArgumentList*    methodArgs;
    CExpressionList*        expList;
    IExpression*            exp;
}

%left '!' "||" "&&"
%left '<' '='
%left '+' '-'
%left '*' '/' '%'
%left '.' '[' ']'

%token CLASS
%token PUBLIC
%token PRIVATE
%token STATIC
%token VOID
%token MAIN
%token EXTENDS
%token RETURN
%token ELSE
%token IF
%token WHILE
%token SOUT
%token LENGTH
%token NEW
%token THIS
%token <sval> ID
%token <ival> INTEGER_LITERAL
%token <bval> LOGIC_LITERAL
%token STRING
%token BOOLEAN
%token INT

%precedence WHILE IF SOUT ID
%precedence CONDITIONAL_STATEMENT WHILE_LOOP_STATEMENT

%type <program>     Program;
%type <mainClass>   MainClass;
%type <classDecls>  ClassDeclarations;
%type <classDecl>   ClassDeclaration;
%type <statements>  Statements;
%type <statement>   Statement;
%type <varDecls>    VarDeclarations;
%type <varDecl>     VarDeclaration;
%type <methodDecls> MethodDeclarations;
%type <methodDecl>  MethodDeclaration;
%type <type>        Type;
%type <accessMod>   AccessModifier;
%type <methodArg>   MethodArgument;
%type <methodArgs>  MethodArguments MethodArgumentsNonEmpty;
%type <exp>         Expression;
%type <expList>     Expressions ExpressionsNonEmpty;

%%
/*__________ The Grammar Rules Section __________*/

Program:
      MainClass ClassDeclarations { root = new CProgram( $1, $2 ); }
    ;

MainClass:
      CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statements '}' '}'
      { $$ = new CMainClass( new CIdExpression( $2 ), new CIdExpression( $12 ), $15 ); }
    ;

ClassDeclarations:
      %empty                              { $$ = new CClassDeclarationList(); }
    | ClassDeclarations ClassDeclaration  { $$ = $1; $$->Add( $2 ); }
    ;

ClassDeclaration:
      CLASS ID '{' VarDeclarations MethodDeclarations '}'             { $$ = new CClassDeclaration( new CIdExpression( $2 ), $4, $5 ); }
    | CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'  { $$ = new CClassDeclaration( new CIdExpression( $2 ), $6, $7, new CIdExpression( $4 ) ); }
    ;

VarDeclarations:
      %empty                          { $$ = new CVarDeclarationList(); }
    | VarDeclarations VarDeclaration  { $$ = $1; $$->Add( $2 ); }
    ;

VarDeclaration:
      Type ID ';'   { $$ = new CVarDeclaration( $1, new CIdExpression( $2 ) ); }
    ;

MethodDeclarations:
      %empty                                { $$ = new CMethodDeclarationList(); }
    | MethodDeclarations MethodDeclaration  { $$ = $1; $$->Add( $2 ); }
    ;

MethodDeclaration:
      AccessModifier Type ID '(' MethodArguments ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
      { $$ = new CMethodDeclaration( $1, $2, new CIdExpression( $3 ), $5, $8, $9, $11 ); }
    ;

Type:
      INT '['']'  { $$ = new CIntArrayTypeModifier(); }
    | BOOLEAN     { $$ = new CBooleanTypeModifier(); }
    | INT         { $$ = new CIntTypeModifier(); }
    | ID          { $$ = new CIdTypeModifier( new CIdExpression( $1 ) ); }
    ;

AccessModifier:
      PUBLIC    { $$ = new CPublicAccessModifier();  }
    | PRIVATE   { $$ = new CPrivateAccessModifier(); }
    ;

MethodArguments:
      %empty                                      { $$ = new CMethodArgumentList(); }
    | MethodArgumentsNonEmpty                     { $$ = $1; }
    ;

MethodArgumentsNonEmpty:
      MethodArgument                              { $$ = new CMethodArgumentList(); $$->Add( $1 ); }
    | MethodArgumentsNonEmpty ',' MethodArgument  { $$ = $1; $$->Add( $3 ); }
    ;

MethodArgument:
      Type ID              { $$ = new CMethodArgument( $1, new CIdExpression( $2 ) ); }
    ;

Statements:
      %empty               { $$ = new CStatementList(); }
    | Statement Statements { $$ = $2; $$->Add( $1 ); }
    ;

Statement:
      '{' Statements '}'                              { $$ = new CBracesStatement( $2 ); }
    | IF '(' Expression ')' Statement ELSE Statement  { $$ = new CConditionalStatement( $3, $5, $7 ); } %prec CONDITIONAL_STATEMENT
    | WHILE '(' Expression ')' Statement              { $$ = new CWhileLoopStatement( $3, $5 ); } %prec WHILE_LOOP_STATEMENT
    | SOUT '(' Expression ')' ';'                     { $$ = new CPrintStatement( $3 ); }
    | ID '=' Expression ';'                           { $$ = new CAssignIdStatement( new CIdExpression( $1 ), $3 ); }
    | ID '[' Expression ']' '=' Expression ';'        { $$ = new CAssignIdWithIndexStatement( new CIdExpression( $1 ), $3, $6); }
    ;

Expressions:
      %empty              { $$ = new CExpressionList(); }
    | ExpressionsNonEmpty { $$ = $1; }
    ;

ExpressionsNonEmpty:
      Expression                         { $$ = new CExpressionList( $1 ); }
    | ExpressionsNonEmpty ',' Expression { $$ = $1; $$->Add( $3 ); }
    ;

Expression:
      Expression "&&" Expression { $$ = new CBinaryExpression( TOperandType::OT_And,   $1, $3 ); }
    | Expression "||" Expression { $$ = new CBinaryExpression( TOperandType::OT_Or,    $1, $3 ); }
    | Expression '<' Expression  { $$ = new CBinaryExpression( TOperandType::OT_LT,    $1, $3 ); }
    | Expression '+' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Plus,  $1, $3 ); }
    | Expression '-' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Minus, $1, $3 ); }
    | Expression '*' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Times, $1, $3 ); }
    | Expression '/' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Div,   $1, $3 ); }
    | Expression '%' Expression  { $$ = new CBinaryExpression( TOperandType::OT_Mod,   $1, $3 ); }

    | Expression '[' Expression ']'         { $$ = new CBracketExpression( $1, $3 )   ; }
    | Expression '.' LENGTH                 { $$ = new CLengthExpression( $1 )        ; }
    | Expression '.' ID '(' Expressions ')' { $$ = new CMethodExpression( $1, new CIdExpression( $3 ), $5 ); }

    | INTEGER_LITERAL            { $$ = new CNumberExpression( $1 )                    ; }
    | LOGIC_LITERAL              { $$ = new CLogicExpression( $1 )                     ; }
    | ID                         { $$ = new CIdExpression( $1 )                        ; }
    | THIS                       { $$ = new CThisExpression()                          ; }
    | NEW INT '[' Expression ']' { $$ = new CNewArrayExpression( $4 )                  ; }
    | NEW ID '(' ')'             { $$ = new CNewIdExpression( new CIdExpression( $2 ) ); }
    | '!' Expression             { $$ = new CNegateExpression( $2 )                    ; }
    | '(' Expression ')'         { $$ = $2                                             ; }
    ;
%%
