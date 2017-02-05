#include <AST/visitors/Visitor.h>
#include <AST/nodes/Expression.h>

using namespace ASTree;

int CVisitor::generateNodeNextIndex( const std::string& nodeType ) {
    int result = 0;
    auto insertResult = nodeTypeLastUsedIndex.insert( std::make_pair( nodeType, result ) );
    if ( !insertResult.second ) {
        result = ++insertResult.first->second;
    }
    return result;
}

std::string CVisitor::generateNodeName( const std::string& nodeTypeName ) {
    return nodeTypeName + std::to_string( generateNodeNextIndex( nodeTypeName ) );
}

std::string CVisitor::operatorName( const TOperatorType& type ) const {
    std::string result;
    switch ( type ) {
        case TOperatorType::OT_Plus: result = "Plus"; break;
        case TOperatorType::OT_Minus: result = "Minus"; break;
        case TOperatorType::OT_Times: result = "Times"; break;
        case TOperatorType::OT_Div: result = "Div"; break;
        case TOperatorType::OT_Mod: result = "Mod"; break;
        case TOperatorType::OT_LT: result = "Less"; break;
        case TOperatorType::OT_And: result = "And"; break;
        case TOperatorType::OT_Or: result = "Or"; break;
    }
    return result;
}

std::string CVisitor::operatorChar( const TOperatorType& type ) const {
    std::string result;
    switch ( type ) {
        case TOperatorType::OT_Plus: result = "+"; break;
        case TOperatorType::OT_Minus: result = "-"; break;
        case TOperatorType::OT_Times: result = "*"; break;
        case TOperatorType::OT_Div: result = "/"; break;
        case TOperatorType::OT_Mod: result = "%"; break;
        case TOperatorType::OT_LT: result = "<"; break;
        case TOperatorType::OT_And: result = "&&"; break;
        case TOperatorType::OT_Or: result = "||"; break;
    }
    return result;
}

void CVisitor::onNodeEnter( const std::string& nodeName, const CLocation& location ) {
    if ( verbose ) {
        std::cout << margin << nodeEnterMarker << nodeName;
        std::cout << " (" << location.firstLine << " : " << location.firstColumn << ")" << std::endl;
        // margin.push_back( '\t' );
        margin.append( "  " );
    }
}

void CVisitor::onNodeExit( const std::string& nodeName, const CLocation& location ) {
    if ( verbose ) {
        margin.erase( margin.size() - 2 );
        // margin.pop_back();
        std::cout << margin << nodeExitMarker << nodeName;
        std::cout << " (" << location.lastLine << " : " << location.lastColumn << ")" << std::endl;
    }
}
