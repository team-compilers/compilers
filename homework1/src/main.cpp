#include <string>
#include <iostream>
#include <fstream>

#include <PrintVisitor.h>
#include <Statement.h>
#include <Expression.h>
#include <VisitorTarget.h>

CAssignStatement* CreateTestTree() {
    CIdExpression* expressionA = new CIdExpression();
    CBinaryExpression* binaryExpression = new CBinaryExpression();
    CNumberExpression* numberExpressionFive = new CNumberExpression();
    CNumberExpression* numberExpressionThree = new CNumberExpression();
    CAssignStatement* assignStatement = new CAssignStatement();

    expressionA->Name = "a";

    assignStatement->LeftValue = expressionA;
    assignStatement->RightValue = binaryExpression;

    numberExpressionFive->Value = 5;
    numberExpressionThree->Value = 3;

    binaryExpression->LeftOperand = numberExpressionFive;
    binaryExpression->Operation = TOperandType::OT_Plus;
    binaryExpression->RightOperand = numberExpressionThree;

    return assignStatement;
}

int main( int argc, char** argv ) {
    CAssignStatement* treeRoot = CreateTestTree();
    CPrintVisitor printVisitor;
    printVisitor.Visit( treeRoot );
    std::string result = printVisitor.GetTraversalInDotLanguage();
    std::ofstream outStream("example1.gv");
    outStream << result;
    outStream.close();

    return 0;
}
