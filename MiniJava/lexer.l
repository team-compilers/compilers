%{
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
INTEGER_LITERAL -?[1-9]{DIGIT}*|0
LOGIC_LITERAL TRUE|FALSE

%%
%%
