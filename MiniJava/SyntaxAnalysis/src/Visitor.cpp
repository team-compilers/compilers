#include <Visitor.h>
#include <Expression.h>

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

std::string CVisitor::toString( const TOperandType& type ) const {
    std::string result;
    switch ( type ) {
        case TOperandType::OT_Plus: result = "Plus"; break;
        case TOperandType::OT_Minus: result = "Minus"; break;
        case TOperandType::OT_Times: result = "Times"; break;
        case TOperandType::OT_Div: result = "Div"; break;
        case TOperandType::OT_Mod: result = "Mod"; break;
        case TOperandType::OT_LT: result = "Less"; break;
        case TOperandType::OT_And: result = "And"; break;
        case TOperandType::OT_Or: result = "Or"; break;
        case TOperandType::OT_Count: result = "Count"; break;
    }
    return result;
}

void CVisitor::onNodeEnter( const std::string& nodeName ) {
    if ( verbose ) {
        std::cout << margin << nodeEnterMarker << nodeName << std::endl;
        // margin.push_back( '\t' );
        margin.append( "  " );
    }
}

void CVisitor::onNodeExit( const std::string& nodeName ) {
    if ( verbose ) {
        margin.erase( margin.size() - 2 );
        // margin.pop_back();
        std::cout << margin << nodeExitMarker << nodeName << std::endl;
    }
}
