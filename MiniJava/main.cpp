#include <fstream>
#include <FlexLexer.h>
int main()
{
	std::ifstream inputStream;
	inputStream.open("Samples/BubbleSort.java");
	std::ofstream outputStream("TestResult.txt");

	yyFlexLexer l(inputStream, outputStream);
	while (l.yylex()!=EOF){}

	inputStream.close();
	outputStream.close();
	return 0;
}