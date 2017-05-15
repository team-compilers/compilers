#pragma once

#include <Synthesis/nodes/VisitorTarget.h>

namespace Synthesis {

// Expressions = arithmetic operations results
class CExpression : public IVisitorTarget {};

class CNullExpression : public CExpression {
public:
    CNullExpression() {}
    ~CNullExpression() {}
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

class CAddCommand : public CExpression {
public:
    CAddCommand(const CExpression* _leftOperand, const CExpression* _rightOperand) :
        leftOperand(_leftOperand), rightOperand(_rightOperand) {}
    ~CAddCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* leftOperand;
    const CExpression* rightOperand;
};

class CAddConstCommand : public CExpression {
public:
    CAddConstCommand( const CExpression* _expression, const int _constant ) :
        expression( _expression ),
        constant( _constant ) {}
    ~CAddConstCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* expression;
    const int constant;
};

class CSubCommand : public CExpression {
public:
    CSubCommand(const CExpression* _leftOperand, const CExpression* _rightOperand) :
        leftOperand(_leftOperand), rightOperand(_rightOperand) {}
    ~CSubCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:
    const CExpression* leftOperand;
    const CExpression* rightOperand;
};

class CSubConstCommand : public CExpression {
public:
    CSubConstCommand( const CExpression* _expression, const int _constant ) :
        expression( _expression ),
        constant( _constant ) {}
    ~CSubConstCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* expression;
    const int constant;
};

class CMultCommand : public CExpression {
public:
    CMultCommand(const CExpression* _leftOperand, const CExpression* _rightOperand) :
        leftOperand(_leftOperand), rightOperand(_rightOperand) {}
    ~CMultCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* leftOperand;
    const CExpression* rightOperand;
};

class CDivCommand : public CExpression {
public:
    CDivCommand(const CExpression* _leftOperand, const CExpression* _rightOperand) :
        leftOperand(_leftOperand), rightOperand(_rightOperand) {}
    ~CDivCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* leftOperand;
    const CExpression* rightOperand;
};

// Get's value from memory: M[src + offset]
class CLoadCommand : public CExpression {
public:
    CLoadCommand( const CExpression* _source, const int _offset ) :
        source( _source ),
        offset( _offset ) {}
    
    ~CLoadCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* source;
    const int offset;
};

// Statements = operations with memory
class CStatement : public IVisitorTarget {};

// M[dst + offset] = src
class CStoreCommand : public CStatement {
public:
    CStoreCommand( const CExpression* _destination, const int _offset, const CExpression* _source ) :
        destination( _destination ),
        offset( _offset ),
        source( _source ) {}

    ~CStoreCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* destination;
    const int offset;
    const CExpression* source;
};

// M[dst] = M[src]
class CMoveCommand : public CStatement {
public:
    CMoveCommand( const CExpression* _destination, const CExpression* _source ) :
        destination( _destination ),
        source( _source ) {}
    
    ~CMoveCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

private:
    const CExpression* destination;
    const CExpression* source;
};

} // namespace Synthesis
