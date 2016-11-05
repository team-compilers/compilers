%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
// #include <Program.h>

// used to prevent applying name-mangling to the yylex identifier
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE* yyin;
extern int line_num;

void yyerror(const char *s);
%}

/*__________ The Bison Declarations Section __________*/

/*This is the best place to write dependency code required for YYSTYPE and YYLTYPE.
In other words, it’s the best place to define types referenced in %union directives.*/
%code requires {
    #include <AccessModifier.h>
}

/*%parse-param { std::unique_ptr<CProgram>& root }
%parse-param { int* hasError }*/

%error-verbose

%union {
    int                     ival;
    bool                    bval;
    char*                   sval;
    IAccessModifier*        accessMod;
    /*CProgram*               program;
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
    CMethodArgument*        methodArg;
    CMethodArgumentList*    methodArgs;
    CExpressionList*        expList;
    IExpression*            exp;*/
}

// Precedence and associativity of the operators
// http://www.cs.bilkent.edu.tr/~guvenir/courses/CS101/op_precedence.html
%left '[' '.' '('
%right '!'
%left '*' '/' '%'
%left '+' '-'
%left '<'
%left AND
%left OR
%left '='

%token AND
%token OR
%token CLASS
%token PUBLIC
%token PRIVATE
%token STATIC
%token VOID
%token MAIN
%token EXTENDS
%token RETURN
%token IF
%token ELSE
%token WHILE
%token SOUT
%token LENGTH
%token NEW
%token THIS
%token STRING
%token BOOLEAN
%token INT
%token <ival> INTEGER_LITERAL
%token <bval> LOGIC_LITERAL
%token <sval> ID

%type <accessMod>   AccessModifier;
/*%type <program>     Program;
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
%type <methodArg>   MethodArgument;
%type <methodArgs>  MethodArguments MethodArgumentsNonEmpty;
%type <exp>         Expression;
%type <expList>     Expressions ExpressionsNonEmpty;
*/

%%
/*__________ The Grammar Rules Section __________*/
Program:
      MainClass ClassDeclarations { std::cout << "Program\n"; }
    ;

MainClass:
      CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statements '}' '}'
      { std::cout << "MainClass\n"; }
    ;

ClassDeclarations:
      %empty                              { std::cout << "ClDeclsE\n"; }
    | ClassDeclarations ClassDeclaration  { std::cout << "ClDecls\n"; }
    ;

ClassDeclaration:
      CLASS ID '{' VarDeclarations MethodDeclarations '}'             { std::cout << "ClDecl\n"; }
    | CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'  { std::cout << "ClDecl\n"; }
    ;

VarDeclarations:
      %empty                          { std::cout << "VarDeclsE\n"; }
    | VarDeclarations VarDeclaration  { std::cout << "VarDecls\n"; }
    ;

VarDeclaration:
      Type ID ';'   { std::cout << "VarDecl\n"; }
    ;

MethodDeclarations:
      %empty                                { std::cout << "MethDeclsE\n"; }
    | MethodDeclarations MethodDeclaration  { std::cout << "MethDecls\n"; }
    ;

MethodDeclaration:
      AccessModifier Type ID '(' MethodArguments ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
      { std::cout << "MethDecl\n"; }
    ;

Type:
      INT '['']'  { std::cout << "IntArr\n"; }
    | BOOLEAN     { std::cout << "Bool\n"; }
    | INT         { std::cout << "Int\n"; }
    | ID          { std::cout << "ID\n"; }
    ;

AccessModifier:
      PUBLIC    { $$ = new CPublicAccessModifier();  }
    | PRIVATE   { $$ = new CPrivateAccessModifier(); }
    ;

MethodArguments:
      %empty                                      { std::cout << "MethArgsE\n"; }
    | MethodArgumentsNonEmpty                     { std::cout << "MethArgs\n"; }
    ;

MethodArgumentsNonEmpty:
      MethodArgument                              { std::cout << "MethArgsNE\n"; }
    | MethodArgumentsNonEmpty ',' MethodArgument  { std::cout << "MethArgsNE\n"; }
    ;

MethodArgument:
      Type ID              { std::cout << "MethArg\n"; }
    ;

Statements:
      %empty               { std::cout << "StatsE\n"; }
    | Statement Statements { std::cout << "Stats\n"; }
    ;

Statement:
      '{' Statements '}'                              { std::cout << "Stat\n"; }
    | IF '(' Expression ')' Statement ELSE Statement  { std::cout << "Stat\n"; }
    | WHILE '(' Expression ')' Statement              { std::cout << "Stat\n"; }
    | SOUT '(' Expression ')' ';'                     { std::cout << "Stat\n"; }
    | ID '=' Expression ';'                           { std::cout << "Stat\n"; }
    | ID '[' Expression ']' '=' Expression ';'        { std::cout << "Stat\n"; }
    ;

Expressions:
      %empty              { std::cout << "ExprsE\n"; }
    | ExpressionsNonEmpty { std::cout << "Exprs\n"; }
    ;

ExpressionsNonEmpty:
      Expression                         { std::cout << "ExprsNE\n"; }
    | ExpressionsNonEmpty ',' Expression { std::cout << "ExprsNE\n"; }
    ;

Expression:
      Expression AND Expression { std::cout << "ExprAND\n"; }
    | Expression OR Expression { std::cout << "ExprOR\n"; }
    | Expression '<' Expression  { std::cout << "Expr<\n"; }
    | Expression '+' Expression  { std::cout << "Expr+\n"; }
    | Expression '-' Expression  { std::cout << "Expr-\n"; }
    | Expression '*' Expression  { std::cout << "Expr*\n"; }
    | Expression '/' Expression  { std::cout << "Expr/\n"; }
    | Expression '%' Expression  { std::cout << "Expr%\n"; }

    | Expression '[' Expression ']'         { std::cout << "Expr\n"; }
    | Expression '.' LENGTH                 { std::cout << "Expr\n"; }
    | Expression '.' ID '(' Expressions ')' { std::cout << "Expr\n"; }

    | IntegerLiteral             { std::cout << "ExprInt\n"; }
    | LOGIC_LITERAL              { std::cout << "ExprLog\n"; }
    | ID                         { std::cout << "ExprId\n"; }
    | THIS                       { std::cout << "ExprThis\n"; }
    | NEW INT '[' Expression ']' { std::cout << "ExprNewArr\n"; }
    | NEW ID '(' ')'             { std::cout << "ExprNewId\n"; }
    | '!' Expression             { std::cout << "ExprNot\n"; }
    | '(' Expression ')'         { std::cout << "ExprParen\n"; }
    ;

IntegerLiteral:
    INTEGER_LITERAL { std::cout << "IntPos\n"; }
    | '-' INTEGER_LITERAL { std::cout << "IntNeg\n"; }
    ;
%%
