%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
#include <AST/nodes/Program.h>
#include "parser.hpp"

// used to prevent applying name-mangling to the yylex identifier
extern "C" int yylex();
extern "C" FILE* yyin;

using namespace ASTree;

void yyerror( CProgram** root, const char* message ) {
    std::cout << "Parse error at line " << yylloc.first_line << ".  Message: " << message << std::endl;
}

CLocation ToCLocation( YYLTYPE yylloc ) {
    return CLocation( yylloc.first_line, yylloc.last_line, 
        yylloc.first_column, yylloc.last_column );
}
%}

/*__________ The Bison Declarations Section __________*/

// This is the best place to write dependency code required for YYSTYPE and YYLTYPE.
// In other words, it’s the best place to define types referenced in %union directives.
%code requires {
    #include <AST/nodes/AccessModifier.h>
    #include <AST/nodes/Expression.h>
    #include <AST/nodes/ExpressionList.h>
    #include <AST/nodes/Statement.h>
    #include <AST/nodes/StatementList.h>
    #include <AST/nodes/TypeModifier.h>
    #include <AST/nodes/VarDeclaration.h>
    #include <AST/nodes/VarDeclarationList.h>
    #include <AST/nodes/MethodArgument.h>
    #include <AST/nodes/MethodArgumentList.h>
    #include <AST/nodes/MethodDeclaration.h>
    #include <AST/nodes/MethodDeclarationList.h>
    #include <AST/nodes/MainClass.h>
    #include <AST/nodes/ClassDeclaration.h>
    #include <AST/nodes/ClassDeclarationList.h>
    #include <AST/nodes/Program.h>
    #include <AST/nodes/VisitorTarget.h>
    using namespace ASTree;
}

// Declare that one or more argument-declaration are additional yyparse arguments.
// The argument-declaration is used when declaring functions or prototypes.
// The last identifier in argument-declaration must be the argument name.
// These are also additional arguments for yyerror function.
%parse-param { CProgram** root }

%locations

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
    CAccessModifier*        accessMod;
    CExpression*            exp;
    CExpressionList*        expList;
    CStatement*             statement;
    CStatementList*         statements;
    CTypeModifier*          type;
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
%left '='
%left OR
%left AND
%left '<'
%left '+' '-'
%left '*' '/' '%'
%right '!'
%left '[' '.' '('

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
      MainClass ClassDeclarations
        { $$ = new CProgram( $1, $2, ToCLocation( @$ ) ); *root = $$; }
    ;

MainClass:
      CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statements '}' '}'
        {
            $$ = new CMainClass(
                new CIdExpression( $2, ToCLocation( @2 ) ),
                new CIdExpression( $12, ToCLocation( @12 ) ),
                $15,
                ToCLocation( @$ )
            );
            free( $2 );
            free( $12 );
        }
    ;

ClassDeclarations:
      %empty
        { $$ = new CClassDeclarationList( ToCLocation( @$ ) ); }
    | ClassDeclarations ClassDeclaration
        { $$ = $1; $$->Add( $2 ); }
    ;

ClassDeclaration:
      CLASS ID '{' VarDeclarations MethodDeclarations '}'
        {
            $$ = new CClassDeclaration( new CIdExpression( $2, ToCLocation( @2 ) ), $4, $5, ToCLocation( @$ ) );
            free( $2 );
        }
    | CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'
        {
            $$ = new CClassDeclaration(
                new CIdExpression( $2, ToCLocation( @2 ) ),
                $6,
                $7,
                new CIdExpression( $4, ToCLocation( @4 ) ),
                ToCLocation( @$ )
            );
            free( $2 );
            free( $4 );
        }
    ;

MethodDeclarations:
      %empty
        { $$ = new CMethodDeclarationList( ToCLocation( @$ ) ); }
    | MethodDeclarations MethodDeclaration
        { $$ = $1; $$->Add( $2 ); }
    ;

MethodDeclaration:
      AccessModifier Type ID '(' MethodArguments ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
        {
            $$ = new CMethodDeclaration(
                $1,
                $2,
                new CIdExpression( $3, ToCLocation( @3 ) ),
                $5,
                $8,
                $9,
                $11,
                ToCLocation( @$ )
            );
            free( $3 );
        }
    ;

VarDeclarations:
      %empty
        { $$ = new CVarDeclarationList( ToCLocation( @$ ) ); }
    | VarDeclarations VarDeclaration
        { $$ = $1; $$->Add( $2 ); }
    ;

VarDeclaration:
      Type ID ';'
        {
            $$ = new CVarDeclaration( $1, new CIdExpression( $2, ToCLocation( @2 ) ), ToCLocation( @$ ) );
            free( $2 );
        }
    ;

Type:
      INT '['']'
        { $$ = new CIntArrayTypeModifier( ToCLocation( @$ ) ); }
    | BOOLEAN
        { $$ = new CBooleanTypeModifier( ToCLocation( @$ ) ); }
    | INT
        { $$ = new CIntTypeModifier( ToCLocation( @$ ) ); }
    | ID
        {
            $$ = new CIdTypeModifier( new CIdExpression( $1, ToCLocation( @1 ) ), ToCLocation( @$ ) );
            free( $1 );
        }
    ;

AccessModifier:
      PUBLIC
        { $$ = new CPublicAccessModifier( ToCLocation( @$ ) ); }
    | PRIVATE
        { $$ = new CPrivateAccessModifier( ToCLocation( @$ ) ); }
    ;

MethodArguments:
      %empty
        { $$ = new CMethodArgumentList( ToCLocation( @$ ) ); }
    | MethodArgumentsNonEmpty
        { $$ = $1; }
    ;

MethodArgumentsNonEmpty:
      MethodArgument
        { $$ = new CMethodArgumentList( ToCLocation( @$ ) ); $$->Add( $1 ); }
    | MethodArgumentsNonEmpty ',' MethodArgument
        { $$ = $1; $$->Add( $3 ); }
    ;

MethodArgument:
      Type ID
        {
            $$ = new CMethodArgument( $1, new CIdExpression( $2, ToCLocation( @2 ) ), ToCLocation( @$ ) );
            free( $2 );
        }
    ;

// statements have to be reversed in every visitor
Statements:
      %empty
        { $$ = new CStatementList( ToCLocation( @$ ) ); }
    | Statement Statements
        { $$ = $2; $$->Add( $1 ); }
    ;

Statement:
      '{' Statements '}'
        { $$ = new CBracesStatement( $2, ToCLocation( @$ ) ); }
    | IF '(' Expression ')' Statement ELSE Statement
        { $$ = new CConditionalStatement( $3, $5, $7, ToCLocation( @$ ) ); }
    | WHILE '(' Expression ')' Statement
        { $$ = new CWhileLoopStatement( $3, $5, ToCLocation( @$ ) ); }
    | SOUT '(' Expression ')' ';'
        { $$ = new CPrintStatement( $3, ToCLocation( @$ ) ); }
    | ID '=' Expression ';'
        {
            $$ = new CAssignIdStatement( new CIdExpression( $1, ToCLocation( @$ ) ), $3, ToCLocation( @$ ) );
            free( $1 );
        }
    | ID '[' Expression ']' '=' Expression ';'
        {
            $$ = new CAssignIdWithIndexStatement( new CIdExpression( $1, ToCLocation( @$ ) ), $3, $6, ToCLocation( @$ ));
            free( $1 );
        }
    ;

Expressions:
      %empty
        { $$ = new CExpressionList( ToCLocation( @$ ) ); }
    | ExpressionsNonEmpty
        { $$ = $1; }
    ;

ExpressionsNonEmpty:
      Expression
        { $$ = new CExpressionList( $1, ToCLocation( @$ ) ); }
    | ExpressionsNonEmpty ',' Expression
        { $$ = $1; $1->Add( $3 ); }
    ;

Expression:
      Expression AND Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_And,   $1, $3, ToCLocation( @$ ) ); }
    | Expression OR Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_Or,    $1, $3, ToCLocation( @$ ) ); }
    | Expression '<' Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_LT,    $1, $3, ToCLocation( @$ ) ); }
    | Expression '+' Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_Plus,  $1, $3, ToCLocation( @$ ) ); }
    | Expression '-' Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_Minus, $1, $3, ToCLocation( @$ ) ); }
    | Expression '*' Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_Times, $1, $3, ToCLocation( @$ ) ); }
    | Expression '/' Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_Div,   $1, $3, ToCLocation( @$ ) ); }
    | Expression '%' Expression
        { $$ = new CBinaryExpression( TOperatorType::OT_Mod,   $1, $3, ToCLocation( @$ ) ); }

    | Expression '[' Expression ']'
        { $$ = new CBracketExpression( $1, $3, ToCLocation( @$ ) ); }
    | Expression '.' LENGTH
        { $$ = new CLengthExpression( $1, ToCLocation( @$ ) ); }
    | Expression '.' ID '(' Expressions ')'
        {
            $$ = new CMethodExpression( $1, new CIdExpression( $3, ToCLocation( @3 ) ), $5, ToCLocation( @$ ) );
            free( $3 );
        }

    | IntegerLiteral
        { $$ = new CNumberExpression( $1, ToCLocation( @$ ) ); }
    | LOGIC_LITERAL
        { $$ = new CLogicExpression( $1, ToCLocation( @$ ) ); }
    | ID
        {
            $$ = new CIdExpression( $1, ToCLocation( @$ ) );
            free( $1 );
        }
    | THIS
        { $$ = new CThisExpression( ToCLocation( @$ ) ); }
    | NEW INT '[' Expression ']'
        { $$ = new CNewArrayExpression( $4, ToCLocation( @$ ) ); }
    | NEW ID '(' ')'
        {
            $$ = new CNewIdExpression( new CIdExpression( $2, ToCLocation( @2 ) ), ToCLocation( @$ ) );
            free( $2 );
        }
    | '!' Expression
        { $$ = new CNegateExpression( $2, ToCLocation( @$ ) ); }
    | '(' Expression ')'
        { $$ = $2; }
    ;

IntegerLiteral:
      INTEGER_LITERAL
        { $$ = $1; }
    | '-' INTEGER_LITERAL
        { $$ = -$2; }
    ;
%%
/*__________ The C Code Section __________*/
// moved to main.cpp
