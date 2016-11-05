%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
#include <Program.h>

// used to prevent applying name-mangling to the yylex identifier
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE* yyin;

// these are defined in main.cpp
extern std::unique_ptr<const CProgram> astRoot;
extern void yyerror( const char* message );
%}

/*__________ The Bison Declarations Section __________*/

// This is the best place to write dependency code required for YYSTYPE and YYLTYPE.
// In other words, it’s the best place to define types referenced in %union directives.
%code requires {
    #include <AccessModifier.h>
    #include <Expression.h>
    #include <ExpressionList.h>
    #include <Statement.h>
    #include <StatementList.h>
    #include <TypeModifier.h>
    #include <VarDeclaration.h>
    #include <VarDeclarationList.h>
    #include <MethodArgument.h>
    #include <MethodArgumentList.h>
    #include <MethodDeclaration.h>
    #include <MethodDeclarationList.h>
    #include <MainClass.h>
    #include <ClassDeclaration.h>
    #include <ClassDeclarationList.h>
    #include <Program.h>
}

/*%parse-param { std::unique_ptr<CProgram>& root }
%parse-param { int* hasError }*/

%error-verbose

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype". But tokens could be of any
// arbitrary data type! So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
    int                     ival;
    bool                    bval;
    char*                   sval;
    IAccessModifier*        accessMod;
    IExpression*            exp;
    CExpressionList*        expList;
    IStatement*             statement;
    CStatementList*         statements;
    ITypeModifier*          type;
    CVarDeclaration*        varDecl;
    CVarDeclarationList*    varDecls;
    CMethodArgument*        methodArg;
    CMethodArgumentList*    methodArgs;
    CMethodDeclaration*     methodDecl;
    CMethodDeclarationList* methodDecls;
    CMainClass*             mainClass;
    CClassDeclaration*      classDecl;
    CClassDeclarationList*  classDecls;
    CProgram*               program;
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

// define the constant-string tokens:
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
// define the "terminal symbol" token types we're going to use (in CAPS by convention),
// and associate each with a field of the union:
%token <ival> INTEGER_LITERAL
%token <bval> LOGIC_LITERAL
%token <sval> ID

// define types for all the nonterminal symbols
%type <accessMod>   AccessModifier;
%type <ival>        IntegerLiteral;
%type <exp>         Expression;
%type <expList>     Expressions ExpressionsNonEmpty;
%type <statement>   Statement;
%type <statements>  Statements;
%type <type>        Type;
%type <varDecl>     VarDeclaration;
%type <varDecls>    VarDeclarations;
%type <methodArg>   MethodArgument;
%type <methodArgs>  MethodArguments MethodArgumentsNonEmpty;
%type <methodDecl>  MethodDeclaration;
%type <methodDecls> MethodDeclarations;
%type <mainClass>   MainClass;
%type <classDecl>   ClassDeclaration;
%type <classDecls>  ClassDeclarations;
%type <program>     Program;

%%
/*__________ The Grammar Rules Section __________*/
Program:
      MainClass ClassDeclarations { std::cout << "Program\n"; $$ = new CProgram( $1, $2 ); astRoot = std::unique_ptr<const CProgram>($$); }
    ;

MainClass:
      CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statements '}' '}'
      { std::cout << "MainClass\n"; $$ = new CMainClass( new CIdExpression( $2 ), new CIdExpression( $12 ), $15 ); }
    ;

ClassDeclarations:
      %empty                              { std::cout << "ClDeclsE\n"; $$ = new CClassDeclarationList(); }
    | ClassDeclarations ClassDeclaration  { std::cout << "ClDecls\n"; $$ = $1; $$->Add( $2 ); }
    ;

ClassDeclaration:
      CLASS ID '{' VarDeclarations MethodDeclarations '}'             { std::cout << "ClDecl\n"; $$ = new CClassDeclaration( new CIdExpression( $2 ), $4, $5 ); }
    | CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'  { std::cout << "ClDecl\n"; $$ = new CClassDeclaration( new CIdExpression( $2 ), $6, $7, new CIdExpression( $4 ) ); }
    ;

MethodDeclarations:
      %empty                                { std::cout << "MethDeclsE\n"; $$ = new CMethodDeclarationList(); }
    | MethodDeclarations MethodDeclaration  { std::cout << "MethDecls\n"; $$ = $1; $$->Add( $2 ); }
    ;

MethodDeclaration:
      AccessModifier Type ID '(' MethodArguments ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
      { std::cout << "MethDecl\n"; $$ = new CMethodDeclaration( $1, $2, new CIdExpression( $3 ), $5, $8, $9, $11 ); }
    ;

VarDeclarations:
      %empty                          { std::cout << "VarDeclsE\n"; $$ = new CVarDeclarationList(); }
    | VarDeclarations VarDeclaration  { std::cout << "VarDecls\n"; $$ = $1; $$->Add( $2 ); }
    ;

VarDeclaration:
      Type ID ';'   { std::cout << "VarDecl\n"; $$ = new CVarDeclaration( $1, new CIdExpression( $2 ) ); }
    ;

Type:
      INT '['']'  { std::cout << "IntArr\n"; $$ = new CIntArrayTypeModifier(); }
    | BOOLEAN     { std::cout << "Bool\n"; $$ = new CBooleanTypeModifier(); }
    | INT         { std::cout << "Int\n"; $$ = new CIntTypeModifier(); }
    | ID          { std::cout << "ID\n"; $$ = new CIdTypeModifier( new CIdExpression( $1 ) ); }
    ;

AccessModifier:
      PUBLIC    { std::cout << "Public\n"; $$ = new CPublicAccessModifier(); }
    | PRIVATE   { std::cout << "Private\n"; $$ = new CPrivateAccessModifier(); }
    ;

MethodArguments:
      %empty                  { std::cout << "MethArgsE\n"; $$ = new CMethodArgumentList(); }
    | MethodArgumentsNonEmpty { std::cout << "MethArgs\n"; $$ = $1; }
    ;

MethodArgumentsNonEmpty:
      MethodArgument                             { std::cout << "MethArgsNE\n"; $$ = new CMethodArgumentList(); $$->Add( $1 ); }
    | MethodArgumentsNonEmpty ',' MethodArgument { std::cout << "MethArgsNE\n"; $$ = $1; $$->Add( $3 ); }
    ;

MethodArgument:
      Type ID              { std::cout << "MethArg\n"; $$ = new CMethodArgument( $1, new CIdExpression( $2 ) ); }
    ;

Statements:
      %empty               { std::cout << "StatsE\n"; $$ = new CStatementList(); }
    | Statement Statements { std::cout << "Stats\n"; $$ = $2; $$->Add( $1 );    }
    ;

Statement:
      '{' Statements '}'                              { std::cout << "Stat\n"; $$ = new CBracesStatement( $2 );                                        }
    | IF '(' Expression ')' Statement ELSE Statement  { std::cout << "Stat\n"; $$ = new CConditionalStatement( $3, $5, $7 );                           }
    | WHILE '(' Expression ')' Statement              { std::cout << "Stat\n"; $$ = new CWhileLoopStatement( $3, $5 );                                 }
    | SOUT '(' Expression ')' ';'                     { std::cout << "Stat\n"; $$ = new CPrintStatement( $3 );                                         }
    | ID '=' Expression ';'                           { std::cout << "Stat\n"; $$ = new CAssignIdStatement( new CIdExpression( $1 ), $3 );             }
    | ID '[' Expression ']' '=' Expression ';'        { std::cout << "Stat\n"; $$ = new CAssignIdWithIndexStatement( new CIdExpression( $1 ), $3, $6); }
    ;

Expressions:
      %empty              { std::cout << "ExprsE\n"; $$ = new CExpressionList(); }
    | ExpressionsNonEmpty { std::cout << "Exprs\n";  $$ = $1; }
    ;

ExpressionsNonEmpty:
      Expression                         { std::cout << "ExprsNE\n"; $$ = new CExpressionList( $1 ); }
    | ExpressionsNonEmpty ',' Expression { std::cout << "ExprsNE\n"; $$ = $1; $1->Add( $3 );         }
    ;

Expression:
      Expression AND Expression  { std::cout << "ExprAND\n"; $$ = new CBinaryExpression( TOperandType::OT_And,   $1, $3 ); }
    | Expression OR Expression   { std::cout << "ExprOR\n";  $$ = new CBinaryExpression( TOperandType::OT_Or,    $1, $3 ); }
    | Expression '<' Expression  { std::cout << "Expr<\n";   $$ = new CBinaryExpression( TOperandType::OT_LT,    $1, $3 ); }
    | Expression '+' Expression  { std::cout << "Expr+\n";   $$ = new CBinaryExpression( TOperandType::OT_Plus,  $1, $3 ); }
    | Expression '-' Expression  { std::cout << "Expr-\n";   $$ = new CBinaryExpression( TOperandType::OT_Minus, $1, $3 ); }
    | Expression '*' Expression  { std::cout << "Expr*\n";   $$ = new CBinaryExpression( TOperandType::OT_Times, $1, $3 ); }
    | Expression '/' Expression  { std::cout << "Expr/\n";   $$ = new CBinaryExpression( TOperandType::OT_Div,   $1, $3 ); }
    | Expression '%' Expression  { std::cout << "Expr%\n";   $$ = new CBinaryExpression( TOperandType::OT_Mod,   $1, $3 ); }

    | Expression '[' Expression ']'         { std::cout << "Expr\n"; $$ = new CBracketExpression( $1, $3 ); }
    | Expression '.' LENGTH                 { std::cout << "Expr\n"; $$ = new CLengthExpression( $1 )     ; }
    | Expression '.' ID '(' Expressions ')' { std::cout << "Expr\n"; $$ = new CMethodExpression( $1, new CIdExpression( $3 ), $5 ); }

    | IntegerLiteral             { std::cout << "ExprInt{"<<$1<<"}\n"; $$ = new CNumberExpression( $1 )                    ; }
    | LOGIC_LITERAL              { std::cout << "ExprLog{"<<($1 ? "true" : "false")<<"}\n"; $$ = new CLogicExpression( $1 )                     ; }
    | ID                         { std::cout << "ExprId{"<<$1<<"}\n"; $$ = new CIdExpression( $1 )                        ; }
    | THIS                       { std::cout << "ExprThis\n"; $$ = new CThisExpression()                          ; }
    | NEW INT '[' Expression ']' { std::cout << "ExprNewArr\n"; $$ = new CNewArrayExpression( $4 )                  ; }
    | NEW ID '(' ')'             { std::cout << "ExprNewId\n"; $$ = new CNewIdExpression( new CIdExpression( $2 ) ); }
    | '!' Expression             { std::cout << "ExprNot\n"; $$ = new CNegateExpression( $2 )                    ; }
    | '(' Expression ')'         { std::cout << "ExprParen\n"; $$ = $2                                             ; }
    ;

IntegerLiteral:
      INTEGER_LITERAL     { std::cout << "IntPos\n"; $$ = $1;  }
    | '-' INTEGER_LITERAL { std::cout << "IntNeg\n"; $$ = -$2; }
    ;
%%
/*__________ The C Code Section __________*/
// moved to main.cpp
