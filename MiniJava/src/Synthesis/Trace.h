#pragma once

#include <vector>

#include <IRT/nodes/Statement.h>

namespace Synthesis {

using CBlock = IRTree::CStatementList;
using CTrace = std::vector<std::unique_ptr<const CBlock>>;

std::unique_ptr<CTrace> RearrangeBlocks( std::unique_ptr<CTrace> trace );

}
