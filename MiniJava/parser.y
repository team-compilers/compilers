%{
/*__________ The C Declarations Section __________*/
#include <iostream>
#include <memory>
#include "TokenPrinter.h"
extern "C" int yylex();
using std::make_shared;
using std::shared_ptr;

void yyerror( std::shared_ptr<CProgram>&, int*, const char *);
%}

/*__________ The Bison Declarations Section __________*/

/*This is the best place to write dependency code required for YYSTYPE and YYLTYPE. 
In other words, it’s the best place to define types referenced in %union directives.*/
%code requires { #include <common.h> }
%code requires { #include <memory> }

%parse-param { std::shared_ptr<CProgram>& root }
%parse-param { int* hasError }

%error-verbose

%union{
	int 					ival;
	char 					sval[255];
	CProgram* 				program;
	CMainClass* 			mainClass;
	CClassDeclarationList*	classDecls;
	CClassDeclaration* 		classDecl;
	CStatementList* 		statements;
	IStatement* 			statement;
	CVarDeclarationList* 	varDecls;
	CVarDeclaration* 		varDecl;
	CMethodDeclarationList*	methodDecls;
	CMethodDeclaration* 	methodDecl;
	CType* 					type;
	CFormalList* 			formalList;
	CExpressionList* 		expList;
	IExpression* 			exp;
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
%token TRUE
%token FALSE
%token NEW
%token THIS
%token <sval> ID
%token <ival> INTEGER_LITERAL
%token STRING
%token BOOLEAN
%token INT

%type <program> 	Program
%type <mainClass> 	MainClass
%type <classDecls> 	ClassDeclarations
%type <classDecl> 	ClassDeclaration
%type <statements> 	Statements
%type <statement> 	Statement
%type <varDecls> 	VarDeclarations
%type <varDecl> 	VarDeclaration
%type <methodDecls> MethodDeclarations
%type <methodDecl> 	MethodDeclaration
%type <type> 		Type
%type <formalList> 	FormalList FormalRest
%type <expList> 	ExpressionList ExpressionRest
%type <exp> 		Expression

%%
/*__________ The Grammar Rules Section __________*/

Program:
	  MainClass ClassDeclarations <<EOF>>
	;

MainClass:
	  CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statement '}' '}'
	;

ClassDeclaration:
	  CLASS ID '{' VarDeclarations MethodDeclarations '}'
	| CLASS ID EXTENDS ID '{' VarDeclarations MethodDeclarations '}'
	;

VarDeclaration:
	  Type ID ';'
	;

MethodDeclaration:
	  PUBLIC Type ID '(' ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
	| PRIVATE Type ID '(' ')' '{' VarDeclarations Statements RETURN Expression ';' '}'
	;

Type:
	  INT '['']' 	
	| BOOLEAN 	
	| INT 		
	| ID
	;

Statement:
	  '{' Statements '}' 							
	| 'if' '(' Expression ')' Statement ELSE Statement 	
	| WHILE '(' Expression ')' Statement 				
	| SOUT '(' Expression ')' ';'						
	| ID '=' Expression ';'								
	| ID '[' Expression ']' '=' Expression ';'
	;

Expression:
      Expression "&&" Expression 			
	| Expression "||" Expression 			
	| Expression '<' Expression				
	| Expression '+' Expression 			
	| Expression '+' Expression 			
	| Expression '-' Expression 			
	| Expression '*' Expression 			
	| Expression '%' Expression 			
	| Expression '[' Expression ']' 		
	| Expression '.' LENGTH  		
	| Expression '.' ID '(' ')' 		
	| INTEGER_LITERAL			
	| TRUE					
	| FALSE					
	| ID 						
	| THIS 					
	| NEW INT '[' Expression ']'		
	| NEW ID '(' ')'			
	| '!' Expression 				
	| '(' Expression ')'
	;
%%
