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
	int 				ival;
	char 				sval[255];
	CProgram* 			program;
	CMainClass* 		mainClass;
	CClassDeclList* 	classDecls;
	CClassDecl* 		classDecl;
	CStatementList* 	statements;
	IStatement* 		statement;
	CVarDeclList* 		varDecls;
	CVarDecl* 			varDecl;
	CMethodDeclList* 	methodDecls;
	CMethodDecl* 		methodDecl;
	CType* 				type;
	CFormalList* 		formalList;
	CExpList* 			expList;
	IExp* 				exp;
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
%type <classDecls> 	ClassDecls
%type <classDecl> 	ClassDecl
%type <statements> 	Statements
%type <statement> 	Statement
%type <varDecls> 	VarDecls
%type <varDecl> 	VarDecl
%type <methodDecls> MethodDecls
%type <methodDecl> 	MethodDecl
%type <type> 		Type
%type <formalList> 	FormalList FormalRest
%type <expList> 	ExpList ExpRest
%type <exp> 		Exp

%%
/*__________ The Grammar Rules Section __________*/

Program:
	MainClass ClassDecls <<EOF>>
	;

MainClass:
	CLASS ID '{' PUBLIC STATIC VOID MAIN '(' STRING '['']' ID ')' '{' Statement '}' '}'
	;

ClassDecl:
	CLASS ID '{' VarDecls MethodDecls '}' |
	CLASS ID EXTENDS ID '{' VarDecls MethodDecls '}'
	;

VarDecl:
	Type ID ';'
	;

MethodDecl:
	PUBLIC Type ID '(' ')' '{' VarDecls Statements RETURN Exp ';' '}' |
	PRIVATE Type ID '(' ')' '{' VarDecls Statements RETURN Exp ';' '}' |
	;

Type:
	INT '['']' 	|
	BOOLEAN 	|
	INT 		|
	ID
	;

Statement:
	'{' Statements '}' 							|
	'if' '(' Exp ')' Statement ELSE Statement 	|
	WHILE '(' Exp ')' Statement 				|
	SOUT '(' Exp ')' ';'						|
	ID '=' Exp ';'								|
	ID '[' Exp ']' '=' Exp ';'
	;

Exp:
	Exp "&&" Exp 			|
	Exp "||" Exp 			|
	Exp '<' Exp				|
	Exp '+' Exp 			|
	Exp '+' Exp 			|
	Exp '-' Exp 			|
	Exp '*' Exp 			|
	Exp '%' Exp 			|
	Exp '[' Exp ']' 		|
	Exp '.' LENGTH  		|
	Exp '.' ID '(' ')' 		|
	INTEGER_LITERAL			|
	TRUE					|
	FALSE					|
	ID 						|
	THIS 					|
	NEW INT '[' Exp ']'		|
	NEW ID '(' ')'			|
	'!' Exp 				|
	'(' Exp ')'
	;
%%