%{
#include <cstdio>    
#include <iostream>
#include <string>
#include "parser.hpp"
// used to prevent applying name-mangling to the yylex identifier
#define YY_DECL extern "C" int yylex()
int lineNum = 1;
%}

%option noyywrap

LEX_AND        "&&"
LEX_OR         "||"
LEX_CLASS      "class"
LEX_PUBLIC     "public"
LEX_PRIVATE    "private"
LEX_STATIC     "static"
LEX_VOID       "void"
LEX_MAIN       "main"
LEX_EXTENDS    "extends"
LEX_RETURN     "return"
LEX_IF         "if"
LEX_ELSE       "else"
LEX_WHILE      "while"
LEX_SOUT       "System.out.println"
LEX_LENGTH     "length"
LEX_NEW        "new"
LEX_THIS       "this"
LEX_TRUE       "true"
LEX_FALSE      "false"
LEX_STRING     "String"
LEX_BOOLEAN    "boolean"
LEX_INT        "int"
LEX_DIGIT [0-9]
LEX_LETTER [a-zA-Z_]
LEX_INTEGER_LITERAL [1-9]{LEX_DIGIT}*|0
LEX_LOGIC_LITERAL {LEX_TRUE}|{LEX_FALSE}
LEX_ID {LEX_LETTER}({LEX_DIGIT}|{LEX_LETTER})*
LEX_COMMENT \/\/(.)*\n

%%
{LEX_INTEGER_LITERAL} {
    yylval.ival = atoi( yytext );
    return INTEGER_LITERAL;
}

{LEX_LOGIC_LITERAL} {
    yylval.bval = std::string( yytext ) == "true";
    return LOGIC_LITERAL;
}

{LEX_CLASS} {
    return CLASS;
}

{LEX_PUBLIC} {
    return PUBLIC;
}

{LEX_PRIVATE} {
    return PRIVATE;
}

{LEX_STATIC} {
    return STATIC;
}

{LEX_VOID} {
    return VOID;
}

{LEX_MAIN} {
    return MAIN;
}

{LEX_EXTENDS} {
    return EXTENDS;
}

{LEX_RETURN} {
    return RETURN;
}

{LEX_IF} {
    return IF;
}

{LEX_ELSE} {
    return ELSE;
}

{LEX_WHILE} {
    return WHILE;
}

{LEX_SOUT} {
    return SOUT;
}

{LEX_LENGTH} {
    return LENGTH;
}

{LEX_NEW} {
    return NEW;
}

{LEX_THIS} {
    return THIS;
}

{LEX_STRING} {
    return STRING;
}

{LEX_BOOLEAN} {
    return BOOLEAN;
}

{LEX_INT} {
    return INT;
}

{LEX_OR} { return OR; }

{LEX_AND} { return AND; }

{LEX_ID} {
    // we have to copy because we can't rely on yytext not changing underneath us:
    yylval.sval = strdup( yytext );
    return ID;
}

\n { ++lineNum; }

{LEX_COMMENT} {
    ++lineNum;
};

[!%\(\)*+,-./;<=\[\]\{\}] { return yytext[0]; }

<<EOF>> {
    return EOF;
}

.                ;
%%
