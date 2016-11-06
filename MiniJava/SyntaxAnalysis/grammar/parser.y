%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
#include <Program.h>

// used to prevent applying name-mangling to the yylex identifier
extern "C" int yylex();
extern "C" int yyparse( CProgram** root );
extern "C" FILE* yyin;

// these are defined in main.cpp
extern void yyerror( CProgram** root, const char* message );
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

// Declare that one or more argument-declaration are additional yyparse arguments.
// The argument-declaration is used when declaring functions or prototypes.
// The last identifier in argument-declaration must be the argument name.
// These are also additional arguments for yyerror function.
%parse-param { CProgram** root }

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
      MainClass ClassDeclarations
        { $$ = new CProgram( $1, $2 ); *root = $$; }
    ;

MainClass:
      CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statements '}' '}'
        { $$ = new CMainClass( new CIdExpression( $2 ), new CIdExpression( $12 ), $15 ); }
    ;

ClassDeclarations:
      %empty
        { $$ = new CClassDeclarationList(); }
    | ClassDeclarations ClassDeclaration
        { $$ = $1; $$->Add( $2 ); }
    ;

ClassDeclaration:
      CLASS ID '{' VarDeclarations MethodDeclarations '}'
        { $$ = new CClassDeclaration( new CIdExpression( $2 ), $4, $5 ); }
    | CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'
        { $$ = new CClassDeclaration( new CIdExpression( $2 ), $6, $7, new CIdExpression( $4 ) ); }
    ;

MethodDeclarations:
      %empty
        { $$ = new CMethodDeclarationList(); }
    | MethodDeclarations MethodDeclaration
        { $$ = $1; $$->Add( $2 ); }
    ;

MethodDeclaration:
      AccessModifier Type ID '(' MethodArguments ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
        { $$ = new CMethodDeclaration( $1, $2, new CIdExpression( $3 ), $5, $8, $9, $11 ); }
    ;

VarDeclarations:
      %empty
        { $$ = new CVarDeclarationList(); }
    | VarDeclarations VarDeclaration
        { $$ = $1; $$->Add( $2 ); }
    ;

VarDeclaration:
      Type ID ';'
        { $$ = new CVarDeclaration( $1, new CIdExpression( $2 ) ); }
    ;

Type:
      INT '['']'
        { $$ = new CIntArrayTypeModifier(); }
    | BOOLEAN
        { $$ = new CBooleanTypeModifier(); }
    | INT
        { $$ = new CIntTypeModifier(); }
    | ID
        { $$ = new CIdTypeModifier( new CIdExpression( $1 ) ); }
    ;

AccessModifier:
      PUBLIC
        { $$ = new CPublicAccessModifier(); }
    | PRIVATE
        { $$ = new CPrivateAccessModifier(); }
    ;

MethodArguments:
      %empty
        { $$ = new CMethodArgumentList(); }
    | MethodArgumentsNonEmpty
        { $$ = $1; }
    ;

MethodArgumentsNonEmpty:
      MethodArgument
        { $$ = new CMethodArgumentList(); $$->Add( $1 ); }
    | MethodArgumentsNonEmpty ',' MethodArgument
        { $$ = $1; $$->Add( $3 ); }
    ;

MethodArgument:
      Type ID
        { $$ = new CMethodArgument( $1, new CIdExpression( $2 ) ); }
    ;

// statements have to be reversed in every visitor
Statements:
      %empty
        { $$ = new CStatementList(); }
    | Statement Statements
        { $$ = $2; $$->Add( $1 ); }
    ;

Statement:
      '{' Statements '}'
        { $$ = new CBracesStatement( $2 ); }
    | IF '(' Expression ')' Statement ELSE Statement
        { $$ = new CConditionalStatement( $3, $5, $7 ); }
    | WHILE '(' Expression ')' Statement
        { $$ = new CWhileLoopStatement( $3, $5 ); }
    | SOUT '(' Expression ')' ';'
        { $$ = new CPrintStatement( $3 ); }
    | ID '=' Expression ';'
        { $$ = new CAssignIdStatement( new CIdExpression( $1 ), $3 ); }
    | ID '[' Expression ']' '=' Expression ';'
        { $$ = new CAssignIdWithIndexStatement( new CIdExpression( $1 ), $3, $6); }
    ;

Expressions:
      %empty
        { $$ = new CExpressionList(); }
    | ExpressionsNonEmpty
        { $$ = $1; }
    ;

ExpressionsNonEmpty:
      Expression
        { $$ = new CExpressionList( $1 ); }
    | ExpressionsNonEmpty ',' Expression
        { $$ = $1; $1->Add( $3 ); }
    ;

Expression:
      Expression AND Expression
        { $$ = new CBinaryExpression( TOperandType::OT_And,   $1, $3 ); }
    | Expression OR Expression
        { $$ = new CBinaryExpression( TOperandType::OT_Or,    $1, $3 ); }
    | Expression '<' Expression
        { $$ = new CBinaryExpression( TOperandType::OT_LT,    $1, $3 ); }
    | Expression '+' Expression
        { $$ = new CBinaryExpression( TOperandType::OT_Plus,  $1, $3 ); }
    | Expression '-' Expression
        { $$ = new CBinaryExpression( TOperandType::OT_Minus, $1, $3 ); }
    | Expression '*' Expression
        { $$ = new CBinaryExpression( TOperandType::OT_Times, $1, $3 ); }
    | Expression '/' Expression
        { $$ = new CBinaryExpression( TOperandType::OT_Div,   $1, $3 ); }
    | Expression '%' Expression
        { $$ = new CBinaryExpression( TOperandType::OT_Mod,   $1, $3 ); }

    | Expression '[' Expression ']'
        { $$ = new CBracketExpression( $1, $3 ); }
    | Expression '.' LENGTH
        { $$ = new CLengthExpression( $1 ); }
    | Expression '.' ID '(' Expressions ')'
        { $$ = new CMethodExpression( $1, new CIdExpression( $3 ), $5 ); }

    | IntegerLiteral
        { $$ = new CNumberExpression( $1 ); }
    | LOGIC_LITERAL
        { $$ = new CLogicExpression( $1 ); }
    | ID
        { $$ = new CIdExpression( $1 ); }
    | THIS
        { $$ = new CThisExpression(); }
    | NEW INT '[' Expression ']'
        { $$ = new CNewArrayExpression( $4 ); }
    | NEW ID '(' ')'
        { $$ = new CNewIdExpression( new CIdExpression( $2 ) ); }
    | '!' Expression
        { $$ = new CNegateExpression( $2 ); }
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
