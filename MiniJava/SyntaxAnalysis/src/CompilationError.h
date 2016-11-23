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
private:
    CLocation location;
    std::string message;
};
