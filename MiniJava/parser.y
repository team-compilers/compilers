%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
// #include <Program.h>

// used to prevent applying name-mangling to the yylex identifier
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int line_num;

void yyerror(const char *s);
%}

/*__________ The Bison Declarations Section __________*/

/*This is the best place to write dependency code required for YYSTYPE and YYLTYPE. 
In other words, it’s the best place to define types referenced in %union directives.*/
/*%code requires {
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
}*/

/*%parse-param { std::unique_ptr<CProgram>& root }
%parse-param { int* hasError }*/

%error-verbose

%union {
    int                     ival;
    bool                    bval;
    char*                   sval;
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
    IAccessModifier*        accessMod;
    CMethodArgument*        methodArg;
    CMethodArgumentList*    methodArgs;
    CExpressionList*        expList;
    IExpression*            exp;*/
}

// %left '!' "||" "&&"
// %left '<' '='
// %left '+' '-'
// %left '*' '/' '%'
// %left '.' '[' ']'

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


// %precedence WHILE IF SOUT ID
// %precedence CONDITIONAL_STATEMENT WHILE_LOOP_STATEMENT

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
%type <accessMod>   AccessModifier;
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
      %empty                              { std::cout << "ClDecls\n"; }
    | ClassDeclarations ClassDeclaration  { std::cout << "ClDecls\n"; }
    ;

ClassDeclaration:
      CLASS ID '{' VarDeclarations MethodDeclarations '}'             { std::cout << "ClDecl\n"; }
    | CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'  { std::cout << "ClDecl\n"; }
    ;

VarDeclarations:
      %empty                          { std::cout << "VarDecls\n"; }
    | VarDeclarations VarDeclaration  { std::cout << "VarDecls\n"; }
    ;

VarDeclaration:
      Type ID ';'   { std::cout << "VarDecl\n"; }
    ;

MethodDeclarations:
      %empty                                { std::cout << "MethDecls\n"; }
    | MethodDeclarations MethodDeclaration  { std::cout << "MethDecls\n"; }
    ;

MethodDeclaration:
      AccessModifier Type ID '(' MethodArguments ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
      { std::cout << "MethDecl\n"; }
    ;

Type:
      INT '['']'  { std::cout << "IntArr "; }
    | BOOLEAN     { std::cout << "Bool "; }
    | INT         { std::cout << "Int "; }
    | ID          { std::cout << "ID "; }
    ;

AccessModifier:
      PUBLIC    { std::cout << "Public "; }
    | PRIVATE   { std::cout << "Public "; }
    ;

MethodArguments:
      %empty                                      { std::cout << "MethArgs "; }
    | MethodArgumentsNonEmpty                     { std::cout << "MethArgs "; }
    ;

MethodArgumentsNonEmpty:
      MethodArgument                              { std::cout << "MethArgsNE "; }
    | MethodArgumentsNonEmpty ',' MethodArgument  { std::cout << "MethArgsNE "; }
    ;

MethodArgument:
      Type ID              { std::cout << "MethArg "; }
    ;

Statements:
      %empty               { std::cout << "Stats "; }
    | Statement Statements { std::cout << "Stats "; }
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
      %empty              { std::cout << "Exprs "; }
    | ExpressionsNonEmpty { std::cout << "Exprs "; }
    ;

ExpressionsNonEmpty:
      Expression                         { std::cout << "ExprsNE\n"; }
    | ExpressionsNonEmpty ',' Expression { std::cout << "ExprsNE\n"; }
    ;

Expression:
      Expression AND Expression { std::cout << "Expr "; }
    | Expression OR Expression { std::cout << "Expr "; }
    | Expression '<' Expression  { std::cout << "Expr "; }
    | Expression '+' Expression  { std::cout << "Expr "; }
    | Expression '-' Expression  { std::cout << "Expr "; }
    | Expression '*' Expression  { std::cout << "Expr "; }
    | Expression '/' Expression  { std::cout << "Expr "; }
    | Expression '%' Expression  { std::cout << "Expr "; }

    | Expression '[' Expression ']'         { std::cout << "Expr "; }
    | Expression '.' LENGTH                 { std::cout << "Expr "; }
    | Expression '.' ID '(' Expressions ')' { std::cout << "Expr "; }

    | INTEGER_LITERAL            { std::cout << "Expr "; }
    | LOGIC_LITERAL              { std::cout << "Expr "; }
    | ID                         { std::cout << "Expr "; }
    | THIS                       { std::cout << "Expr "; }
    | NEW INT '[' Expression ']' { std::cout << "Expr "; }
    | NEW ID '(' ')'             { std::cout << "Expr "; }
    | '!' Expression             { std::cout << "Expr "; }
    | '(' Expression ')'         { std::cout << "Expr "; }
    ;
%%

#include <stdio.h>
#include <string>
#include <stdexcept>

void yyerror(const char *s) {
    std::cout << "Parse error at line " << line_num << ".  Message: " << s << std::endl;
    // might as well halt now:
    // exit(-1);
}

void lexicalAnalysis( const std::string& inputFileName, const std::string& outputFileName ) {
    FILE* inputStream = fopen(inputFileName.c_str(), "r");
    if (!inputStream) {
        throw std::invalid_argument( "Cannot open file: " + inputFileName );
    }
    yyin = inputStream;
    do {
        yyparse();
    } while (!feof(yyin));
    // std::ifstream inputStream( inputFileName.c_str() );
    // std::ofstream outputStream( outputFileName.c_str() );
    // if ( inputStream.good() ) {
        // if ( outputStream.good() ) {
        //     yyFlexLexer lexer( inputStream, outputStream );
        //     while ( lexer.yylex() != EOF );
        //     inputStream.close();
        //     outputStream.close();
        // } else {
        //     throw std::invalid_argument( "Cannot open file: " + inputFileName );
        // }
        // yyin = inputStream;
        // yylex();
    // } else {
        // throw std::invalid_argument( "Cannot open file: " + inputFileName );
    // }
}

int main() {
    std::string inputFileName = "tmp/dull.java";
    std::string outputFileName = "tmp/tokenized.txt";
    lexicalAnalysis(inputFileName, outputFileName);
    return 0;
}
