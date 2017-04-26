#pragma once

#include <memory>

#include <IRT/nodes/VisitorTarget.h>

using namespace IRTree;

namespace Synthesis {

class IPattern {
public:
    virtual void Consume(const IVisitorTarget* node) = 0;
};

class CAddPattern : public IPattern {
public:
    void Consume(const IVisitorTarget* node) override;
};

}
