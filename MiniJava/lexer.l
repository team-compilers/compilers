%{
#include "TokenPrinter.h"

TokenPrinter tokenPrinter;

%}

%option noyywrap

CLASS      "class"
PUBLIC     "public"
PRIVATE    "private"
STATIC     "static"
VOID       "void"
MAIN       "main"
EXTENDS    "extends"
RETURN     "return"
IF         "if"
ELSE       "else"
WHILE      "while"
SOUT       "System.out.println"
LENGTH     "length"
TRUE       "true"
FALSE      "false"
NEW        "new"
THIS       "this"
STRING     "String"
BOOLEAN    "boolean"
INT        "int"
DIGIT [0-9]
LETTER [a-zA-Z_]
ID {LETTER}({DIGIT}|{LETTER})*
NUMBER -?[1-9]{DIGIT}*|0

%%
{NUMBER} {
    tokenPrinter.PrintNameWithDefinition( yyout, "INT", yytext );
}

{CLASS} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{PUBLIC} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{PRIVATE} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{STATIC} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{VOID} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{MAIN} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{EXTENDS} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{RETURN} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{IF} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{ELSE} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{WHILE} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{SOUT} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{LENGTH} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{TRUE} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{FALSE} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{NEW} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{THIS} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{STRING} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{BOOLEAN} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
}

{INT} {
    tokenPrinter.PrintSimpleString( yyout, yytext );
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
