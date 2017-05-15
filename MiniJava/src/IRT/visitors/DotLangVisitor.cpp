#include <IRT/visitors/DotLangVisitor.h>

using namespace IRTree;

void CDotLangVisitor::addEdge( const std::string& nodeFromName, const std::string& nodeToName ) {
    treeEdges[nodeFromName].push_back( nodeToName );
}

std::string CDotLangVisitor::GetTraversalInDotLanguage() const {
    std::stringstream sstream;
    sstream << "digraph {" << std::endl;
    sstream << '\t' << "ordering = out;" << std::endl;
    for ( auto it = treeEdges.begin(); it != treeEdges.end(); ++it ) {
        const std::string& fromNode = it->first;
        for ( const std::string& toNode : it->second ) {
            sstream << '\t' << fromNode << " -> " << toNode << ';' << std::endl;
        }
    }
    sstream << '}' << std::endl;

    return sstream.str();
}

void CDotLangVisitor::Clear() {
    treeEdges.clear();
    nodeTypeLastUsedIndex.clear();
    visitedNodeStack.clear();
}


/*__________ Expressions __________*/

void CDotLangVisitor::Visit( const CConstExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CONST );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Value" ) + ": ";
    valueNodeName += std::to_string( expression->Value() ) + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CNameExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NAME );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Label" ) + ": ";
    valueNodeName += expression->Label().ToString() + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CTempExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_TEMP );
    onNodeEnter( nodeName );

    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Temp" ) + ": ";
    valueNodeName += expression->Temporary().ToString() + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    addEdge( nodeName, generateNodeName( operatorName( expression->Operation() ) ) );

    expression->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMemExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_MEM );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->Address()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CCallExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_CALL );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->Function()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->Arguments()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();    

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CEseqExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ESEQ );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    expression->Statement()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    expression->Expression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CDotLangVisitor::Visit( const CExpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_EXP );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->Expression()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CJumpConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CJUMP );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    addEdge( nodeName, generateNodeName( operatorName( statement->Operation() ) ) );

    statement->LeftOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->RightOperand()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    std::string labelTrueNodeName = "\"" + generateNodeName( "Label" ) + ": ";
    labelTrueNodeName += statement->TrueLabel().ToString() + "\"";
    addEdge( nodeName, labelTrueNodeName );

    std::string labelFalseNodeName = "\"" + generateNodeName( "Label" ) + ": ";
    labelFalseNodeName += statement->FalseLabel().ToString() + "\"";
    addEdge( nodeName, labelFalseNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CJumpStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_JUMP );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Label" ) + ": ";
    valueNodeName += statement->Target().ToString() + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CLabelStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LABEL );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    std::string valueNodeName = "\"" + generateNodeName( "Label" ) + ": ";
    valueNodeName += statement->Label().ToString() + "\"";
    addEdge( nodeName, valueNodeName );

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CMoveStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_MOVE );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->Destination()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->Source()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CSeqStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_SEQ );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    statement->LeftStatement()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    statement->RightStatement()->Accept( this );
    addEdge( nodeName, visitedNodeStack.back() );
    visitedNodeStack.pop_back();

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CDotLangVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_EXP );
    onNodeEnter( nodeName );
    visitedNodeStack.push_back( nodeName );

    const std::vector<std::unique_ptr<const CExpression>>& expressions = list->Expressions();
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}

void CDotLangVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::LIST_STAT );
    onNodeEnter( nodeName );

    visitedNodeStack.push_back( nodeName );

    const std::vector<std::unique_ptr<const CStatement>>& statements = list->Statements();
    for ( auto it = statements.begin(); it != statements.end(); ++it ) {
        ( *it )->Accept( this );

        addEdge( nodeName, visitedNodeStack.back() );
        visitedNodeStack.pop_back();
    }

    onNodeExit( nodeName );
}
