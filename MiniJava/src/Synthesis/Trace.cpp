#include <Synthesis/Trace.h>

#include <IRT/visitors/BlockRearrangeVisitor.h>
#include <IRT/visitors/TraceFormationVisitor.h>

#include <unordered_map>
#include <vector>

using namespace Synthesis;

std::unique_ptr<CTrace> Synthesis::RearrangeBlocks( std::unique_ptr<CTrace> trace ) {
    std::unordered_map<std::string, int> blockNameToIndex;
    std::vector<std::string> blockIndexToName;
    std::vector<std::shared_ptr<std::string>> blockIndexToJumpTarget;
    blockIndexToName.reserve( trace->size() );
    blockIndexToJumpTarget.reserve( trace->size() );
    for ( int blockIndex = 0; blockIndex < trace->size(); ++blockIndex ) {
        IRTree::CBlockRearrangeVisitor blockVisitor;

        trace->at( blockIndex )->Accept( &blockVisitor );

        std::string blockName = blockVisitor.LabelName();
        blockNameToIndex.emplace( blockName, blockIndex );
        blockIndexToName.push_back( blockName );

        blockIndexToJumpTarget.push_back( blockVisitor.JumpTargetLabelName() );
    }

    std::vector<bool> visited( trace->size(), false );
    std::unique_ptr<CTrace> traceArranged( new CTrace() );
    traceArranged->reserve( trace->size() );
    for ( int rootBlockIndex = 0; rootBlockIndex < trace->size(); ++rootBlockIndex ) {
        int blockIndex = rootBlockIndex;

        while ( blockIndex >= 0 && !visited[blockIndex] ) {
            visited[blockIndex] = true;
            auto statementPtr = trace->at( blockIndex )->Clone();
            std::unique_ptr<const IRTree::CStatementList> blockPtr(
                reinterpret_cast<const IRTree::CStatementList*>( statementPtr.release() )
            );
            traceArranged->push_back( std::move( blockPtr ) );
            if ( !blockIndexToJumpTarget[blockIndex] ||
                 *blockIndexToJumpTarget.at( blockIndex ) == IRTree::CTraceFormationVisitor::EndBlockLabelName() ) {
                blockIndex = -1;
            } else {
                blockIndex = blockNameToIndex.at( *blockIndexToJumpTarget[blockIndex] );
            }
        }
    }
    return std::move( traceArranged );
}
