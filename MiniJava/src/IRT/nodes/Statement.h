#pragma once

#include <IRT/nodes/VisitorTarget.h>

namespace IRTree {

class IStatement : public IVisitorTarget {
public:
    virtual ~IStatement() {}
};

class CStatement : public IStatement {
public:
    virtual ~CStatement() {}
};

class CLabelStatement : IStatement {

};

} // namespace IRTree
