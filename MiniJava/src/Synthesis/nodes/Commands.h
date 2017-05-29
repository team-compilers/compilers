#pragma once

#include <Synthesis/nodes/VisitorTarget.h>
#include <string>
#include <vector>
#include <IRT/nodes/Expression.h>

namespace Synthesis {

class CCommand : public IVisitorTarget {};

// Expressions = arithmetic operations results, stores result to register
class CExpression : public CCommand {};

class CNullExpression : public CExpression {
public:
    CNullExpression() {}
    ~CNullExpression() {}
    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
};

// Virtual expression resonsible for some register (temp node on the ir-level)
class CTempExpression : public CExpression {
public:
    CTempExpression( std::string _name ) :
        name( _name ) {}
    ~CTempExpression() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const std::string& Name() const { return name; }

private:
    std::string name;
};

class CAddCommand : public CExpression {
public:
    CAddCommand(const CExpression* _leftOperand, const CExpression* _rightOperand) :
        leftOperand(_leftOperand), rightOperand(_rightOperand) {}
    ~CAddCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* LeftOperand() const { return leftOperand; }
    const CExpression* RightOperand() const { return rightOperand; }

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

    const CExpression* Expression() const { return expression; }
    int Constant() const { return constant; }

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

    const CExpression* LeftOperand() const { return leftOperand; }
    const CExpression* RightOperand() const { return rightOperand; }

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

    const CExpression* Expression() const { return expression; }
    int Constant() const { return constant; }

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

    const CExpression* LeftOperand() const { return leftOperand; }
    const CExpression* RightOperand() const { return rightOperand; }

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

    const CExpression* LeftOperand() const { return leftOperand; }
    const CExpression* RightOperand() const { return rightOperand; }

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

    const CExpression* Source() const { return source; }
    int Offset() const { return offset; }

private:
    const CExpression* source;
    const int offset;
};

// Statements - operations without storing result
class CStatement : public CCommand {};

// Memory Statements - stores result to memory cell
class CMemoryStatement : public CStatement {};

// M[dst + offset] = src
class CStoreCommand : public CMemoryStatement {
public:
    CStoreCommand( const CExpression* _destination, const int _offset, const CExpression* _source ) :
        destination( _destination ),
        offset( _offset ),
        source( _source ) {}

    ~CStoreCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* Destination() const { return destination; }
    int Offset() const { return offset; }
    const CExpression* Source() const { return source; }

private:
    const CExpression* destination;
    const int offset;
    const CExpression* source;
};

// dst = src
class CMoveRegisterCommand : public CStatement {
public:
    CMoveRegisterCommand( const CTempExpression* _destination, const CExpression* _source ) :
        destination( _destination ),
        source( _source ) {}

    ~CMoveRegisterCommand() {}

    const CTempExpression* Destination() const { return destination; }
    const CExpression* Source() const { return source; } 

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
private:
    const CTempExpression* destination;
    const CExpression* source;
};

class CLabelDeclarationCommand : public CCommand {
public:
    CLabelDeclarationCommand( std::string _name ) :
        name( _name ) {} 
    ~CLabelDeclarationCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const std::string& Name() const { return name; }

private:
    std::string name;
};

class CCallFunctionCommand : public CExpression {
public:
    CCallFunctionCommand( const std::string& _function,
            const std::vector<const CExpression*>& _arguments ) :
        function( _function ),
        arguments( _arguments ) {}
    
    ~CCallFunctionCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const std::string& Function() const { return function; }
    const std::vector<const Synthesis::CExpression*>& Arguments() const { return arguments; }

private:
    const std::string function;
    const std::vector<const Synthesis::CExpression*> arguments;
};

class CJumpCommand : public CStatement {
public:
    CJumpCommand( std::string _labelName ) : labelName( _labelName ) {}
    ~CJumpCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const std::string& LabelName() const { return labelName; }

private:
    std::string labelName;
};

class CConditionalJumpCommand : public CStatement {
public:
    CConditionalJumpCommand( const CExpression* _leftPart, const CExpression* _rightPart,
            IRTree::TLogicOperatorType _cmp, std::string _positiveLabelName, std::string _negativeLabelName ) :
        leftPart( _leftPart ),
        rightPart( _rightPart ),
        cmp( _cmp ),
        positiveLabelName( _positiveLabelName ),
        negativeLabelName( _negativeLabelName ) {}
    ~CConditionalJumpCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }

    const CExpression* LeftPart() const { return leftPart; }
    const CExpression* RightPart() const { return rightPart; }
    IRTree::TLogicOperatorType Cmp() const { return cmp; }
    const std::string& PositiveLabelName() const { return positiveLabelName; }
    const std::string& NegativeLabelName() const { return negativeLabelName; }

private:
    const CExpression* leftPart;
    const CExpression* rightPart;
    IRTree::TLogicOperatorType cmp;
    std::string positiveLabelName;
    std::string negativeLabelName;
};

class CExpStatementCommand : public CStatement {
public:
    CExpStatementCommand( const CExpression* _expression ) :
        expression( _expression ) {}
    ~CExpStatementCommand() {}

    void Accept( IVisitor* visitor ) const override { visitor->Visit( this ); }
    const CExpression* Expression() const { return expression; }

private:
    const CExpression* expression;
};

} // namespace Synthesis
