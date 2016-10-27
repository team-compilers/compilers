%{
	#include "parser.hpp"
%}

%option noyywrap

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
LEX_TRUE       "true"
LEX_FALSE      "false"
LEX_NEW        "new"
LEX_THIS       "this"
LEX_STRING     "String"
LEX_BOOLEAN    "boolean"
LEX_INT        "int"
LEX_DIGIT [0-9]
LEX_LETTER [a-zA-Z_]
LEX_ID {LEX_LETTER}({LEX_DIGIT}|{LEX_LETTER})*
LEX_INTEGER_LITERAL -?[1-9]{LEX_DIGIT}*|0
LEX_LOGIC_LITERAL LEX_TRUE|LEX_FALSE

%%
{LEX_INTEGER_LITERAL} {
    return INTEGER_LITERAL;
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

{LEX_TRUE} {
    return LOGIC_LITERAL;
}

{LEX_FALSE} {
    return LOGIC_LITERAL;
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

{LEX_ID} {
    return ID;
}

<<EOF>> {
    return EOF;
}
%%
