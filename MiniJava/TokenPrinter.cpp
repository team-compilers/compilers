#include "TokenPrinter.h"

TokenPrinter::TokenPrinter() {
    SetXPosition( 0 );
    SetYPosition( 0 );
}

void TokenPrinter::SetXPosition( int position ) {
    xPosition = position;
}

int TokenPrinter::GetXPosition() const {
    return xPosition;
}

void TokenPrinter::SetYPosition( int position ) {
    yPosition = position;
}

int TokenPrinter::GetYPosition() const {
    return yPosition;
}

void TokenPrinter::PrintLocation( std::ostream& outputStream ) const {
    outputStream << '{' << yPosition << ',' << xPosition << '}' << ' ';
}

void TokenPrinter::PrintSimpleString( std::ostream& outputStream, const char* text ) {
    std::string outString;
    const char* toPrint = text;
    while ( *text != 0 ) {
        outString += std::toupper( *text );
        text++;
    }
    outputStream << outString;
    updateAndPrintLocation( outputStream, toPrint );
}

void TokenPrinter::PrintBracket( std::ostream& outputStream, const char* text ) {
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

void TokenPrinter::PrintOperation( std::ostream& outputStream, const char* operation ) {
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

void TokenPrinter::PrintNameWithDefinition( std::ostream& outputStream, const char* name, const char* definition ) {
    outputStream << name << "(" << definition << ")";
    updateAndPrintLocation( outputStream, definition );
}

void TokenPrinter::PrintEnter( std::ostream& outputStream ) {
    outputStream << '\n';
    xPosition = 0;
    yPosition++;
}

void TokenPrinter::PrintTab( std::ostream& outputStream, const char* yytext ) {
    outputStream << yytext;
    updateLocation( yytext );
}

void TokenPrinter::updateLocation( const char* yytext ) {
    xPosition += std::string( yytext ).size();
}

void TokenPrinter::updateAndPrintLocation( std::ostream& outputStream, const char* yytext ) {
    updateLocation( yytext );
    PrintLocation( outputStream );
}
