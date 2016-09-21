%option noyywrap
%{
    #include <cctype>
    #include <cstring>
    #include <fstream>
    #include <iostream>
    #include <string>

    int xPosition = 0;
    int yPosition = 0;
    char* yytext;

    void updateLocation( const char* yytext ) {
        xPosition += strlen( yytext );
    }

    void printLocation( std::ostream& outputStream ) {
        outputStream << '{' << yPosition << ',' << xPosition << '}' << ' ';
    }

    void updateAndPrintLocation( std::ostream& outputStream, const char* yytext ) {
        updateLocation( yytext );
        printLocation( outputStream );
    }

    void printSimpleString( std::ostream& outputStream, const char* text ) {
        std::string outString;
        const char* toPrint = text;
        while ( *text != 0 ) {
            outString += std::toupper( *text );
            text++;
        }
        outputStream << outString;
        updateAndPrintLocation( outputStream, toPrint );
    }

    void printBracket( std::ostream& outputStream, const char* text ) {
        char bracket = *text;
        std::string outString;
        switch ( bracket ) {
            case '[': outString = "LBRACKET"; break;
            case ']': outString = "RBRACKET"; break;
            case '(': outString = "LPAREN"; break;
            case ')': outString = "RPAREN"; break;
            case '{': outString = "LBRACE"; break;
            case '}': outString = "RBRACE"; break;
        }
        outputStream << outString;
        updateAndPrintLocation( outputStream, text );
    }

    void printOperation( std::ostream& outputStream, const char* operation ) {
        std::string outString;
        if ( operation == "&&" ) {
            outString = "AND";
        } else if ( operation == "||") {
            outString = "OR";
        } else {
            char oper = *operation;
            switch ( oper ) {
                case '+': outString = "PLUS"; break;
                case '-': outString = "MINUS"; break;
                case '*': outString = "STAR"; break;
                case '<': outString = "LESS"; break;
                case '!': outString = "BANG"; break;
                case '%': outString = "MOD"; break;
                case '=': outString = "ASSIGN"; break;
                case '.': outString = "DOT"; break;
                case ',': outString = "COMMA"; break;
                case ';': outString = "SEMI"; break;
            }
        }
        outputStream << outString;
        updateAndPrintLocation( outputStream, operation );
    }

    void printNameWithDefinition( std::ostream& outputStream, const char* name, const char* definition ) {
        outputStream << name << "(" << definition << ")";
        updateAndPrintLocation( outputStream, definition );
    }

    void printEnter( std::ostream& outputStream ) {
        outputStream << '\n';
        xPosition = 0;
        yPosition++;
    }

    void printTab( std::ostream& outputStream, const char* yytext) {
        outputStream << yytext;
        updateLocation( yytext );
    }

%}

DIGIT [0-9]
LETTER [a-zA-Z_]
ID {LETTER}({DIGIT}|{LETTER})*
INTEGER -?[1-9]{DIGIT}*|0
DEFINED "class"|"public"|"private"|"static"|"void"|"main"|"extends"|"return"|"this"|"new"|"if"|"else"|"while"|"true"|"false"|"int"|"boolean"|"System.out.println"
BRACKETS "["|"]"|"("|")"|"{"|"}"
OPERATIONS "+"|"-"|"*"|"<"|"&&"|"||"|"!"|"%"|"="|"."|","|";" 

%%
\/\/(.)*\n {
    xPosition = 0;
    yPosition++;
}

{INTEGER} {
    printNameWithDefinition( yyout, "INT", yytext );
}

{BRACKETS} {
    printBracket( yyout, yytext );
}

{DEFINED} {
    printSimpleString( yyout, yytext );
}

{OPERATIONS} {
    printOperation( yyout, yytext );
}

{ID} {
    printNameWithDefinition( yyout, "ID", yytext );
}

"\n" {
    printEnter( yyout );
}

"\r" {
    printEnter( yyout );
}

"\t" {
    printTab( yyout, yytext );
}

<<EOF>> {
    return EOF;
}
%%
//Empty
