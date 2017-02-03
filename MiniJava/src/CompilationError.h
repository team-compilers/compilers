#pragma once

#include <string>
#include <AST/nodes/VisitorTarget.h>

class CCompilationError {
public:
    CCompilationError( const ASTree::CLocation& _location, const std::string& _message );
    const ASTree::CLocation& Location() const;
    const std::string& Message() const;
    std::string ToString() const;

    static const std::string REDEFINITION_LOCAL_VAR;
    static const std::string REDEFINITION_CLASS;
    static const std::string REDEFINITION_METHOD;
    static const std::string DIFFERENT_TYPES_OF_ARGUMENTS;
    static const std::string INVALID_PRINT_TYPE;
    static const std::string INVALID_CONDITION_TYPE;
    static const std::string INVALID_INDEX_TYPE;
    static const std::string INVALID_LENGTH_TYPE;
    static const std::string CYCLIC_INHERITANCE;
    static const std::string PARENT_CLASS_NOT_EXISTS;
    static const std::string TYPE_NOT_EXISTS;
    static const std::string VAR_UNDEFINED;
    // static const std::string METHOD_OVERLOADING;
    static const std::string CLASS_HAS_NO_METHOD;
    static const std::string INVALID_CALLER_EXPRESSION;
    static const std::string ARGS_NUMBERS_NOT_MATCH;
    static const std::string ARG_TYPE_NOT_MATCH;
    static const std::string METHOD_IS_PRIVATE;

private:
    ASTree::CLocation location;
    std::string message;
};
