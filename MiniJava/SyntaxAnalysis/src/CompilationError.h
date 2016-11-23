#pragma once

#include <string>
#include <VisitorTarget.h>

class CCompilationError {
public:
    CCompilationError( const CLocation& _location, const std::string& _message );
    const CLocation& Location() const;
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

private:
    CLocation location;
    std::string message;
};
