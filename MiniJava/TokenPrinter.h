#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

class TokenPrinter {
public:
    TokenPrinter();
    
    void SetXPosition( int position );
    int GetXPosition() const;
    void SetYPosition( int position );
    int GetYPosition() const;
    void PrintLocation( std::ostream& outputStream ) const;
    void PrintSimpleString( std::ostream& outputStream, const char* text );
    void PrintBracket( std::ostream& outputStream, const char* text );
    void PrintOperation( std::ostream& outputStream, const char* operation );
    void PrintNameWithDefinition( std::ostream& outputStream, const char* name, const char* definition );
    void PrintEnter( std::ostream& outputStream );
    void PrintTab( std::ostream& outputStream, const char* yytext );
private:
    void updateLocation( const char* yytext );
    void updateAndPrintLocation( std::ostream& outputStream, const char* yytext );

    int xPosition;
    int yPosition;
};
