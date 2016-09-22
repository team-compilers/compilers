%{
#include "TokenPrinter.h"

TokenPrinter tokenPrinter;

%}

%option noyywrap

DIGIT [0-9]
LETTER [a-zA-Z_]
ID {LETTER}({DIGIT}|{LETTER})*
INTEGER -?[1-9]{DIGIT}*|0
DEFINED "class"|"public"|"private"|"static"|"void"|"main"|"extends"|"return"|"this"|"new"|"if"|"else"|"while"|"true"|"false"|"int"|"boolean"|"System.out.println"
BRACKETS "["|"]"|"("|")"|"{"|"}"
OPERATIONS "+"|"-"|"*"|"<"|"&&"|"||"|"!"|"%"|"="|"."|","|";" 
COMMENT \/\/(.)*\n

%%
{COMMENT} {
    // comment
    tokenPrinter.SetXPosition( 0 );
    tokenPrinter.SetYPosition( tokenPrinter.GetYPosition() + 1 );
}

{INTEGER} {
    tokenPrinter.PrintNameWithDefinition( yyout, "INT", yytext );
}

{BRACKETS} {
    tokenPrinter.PrintBracket( yyout, yytext );
}

{DEFINED} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{OPERATIONS} {
    tokenPrinter.PrintOperation( yyout, yytext );
}

{ID} {
    tokenPrinter.PrintNameWithDefinition( yyout, "ID", yytext );
}

"\n" {
    tokenPrinter.PrintEnter( yyout );
}

"\r" {
    tokenPrinter.PrintEnter( yyout );
}

"\t" {
    tokenPrinter.PrintTab( yyout, yytext );
}

<<EOF>> {
    tokenPrinter = TokenPrinter();
    return EOF;
}
%%
