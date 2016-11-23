#include <CompilationError.h>
#include <stringstream>

const std::string CCompilationError::REDEFINITION_LOCAL_VAR = "Local variable redefined";
const std::string CCompilationError::REDEFINITION_CLASS = "Class redefined";
static const std::string CCompilationError::REDEFINITION_METHOD = "Method redefined";

CCompilationError::CCompilationError( const CLocation& _location, const std::string& _message ) 
    : location( _location ), message( _message ) 
{}

const CLocation& CCompilationError::Location() const 
{
    return location;
}

const std::string& CCompilationError::Message() const
{
    return message;
}
    
std::string CCompilationError::ToString() const {
    std::stringstream sstream;
    sstream << location.firstLine << ':' << location.firstColumn << ": " << message;
    return sstream.str();
}
