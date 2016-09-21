%option noyywrap
%{
	#include <iostream>
	#include <cstring>
	#include <string>
	#include <cctype>
	#include <fstream>

	int xPosition = 0;
	int yPosition = 0;
	char* yytext;

	std::ofstream outputStream("TestResult.txt");
	void updateLocation( const char* yytext ) {
		xPosition += strlen( yytext );
	}
	void printLocation() {
		outputStream << '{' << yPosition << ',' << xPosition << '}' << ' ';
	}
	void updateAndPrintLocation( const char* yytext) {
		updateLocation( yytext );
		printLocation();
	}
	void printSimpleString( const char* text ) {
		std::string outString;
		const char* toPrint = text;
		while ( *text != 0 ) {
			outString += std::toupper( *text );
			text++;
		}
		outputStream << outString;
		updateAndPrintLocation( toPrint );
	}
	void printBracket( const char* text ) {
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
		updateAndPrintLocation( text );
	}
	void printOperation( const char* operation ) {
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
		updateAndPrintLocation( operation );
	}
	void printNameWithDefinition( const char* name, const char* definition ) {
		outputStream << name << "(" << definition << ")";
		updateAndPrintLocation( definition );
	}
	void printEnter() {
		outputStream << '\n';
		xPosition = 0;
		yPosition++;
	}
	void printTab(const char* yytext) {
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
	printNameWithDefinition("INT", yytext);
}
{BRACKETS} {
	printBracket( yytext );
}
{DEFINED} {
	printSimpleString( yytext );
}
{OPERATIONS} {
	printOperation( yytext );
}
{ID} {
	printNameWithDefinition("ID", yytext);
}
"\n" {
	printEnter();
}
"\r" {
	printEnter();
}
"\t" {
	printTab( yytext );
}
<<EOF>> {
    return EOF;
}
%%
//Empty