#include <CompilationError.h>
#include <sstream>

const std::string CCompilationError::REDEFINITION_LOCAL_VAR = "Local variable redefined";
const std::string CCompilationError::REDEFINITION_CLASS = "Class redefined";
const std::string CCompilationError::REDEFINITION_METHOD = "Method redefined";
const std::string CCompilationError::STATEMENT_DIFFERENT_TYPES = "Statement has different types of left and right part";
const std::string CCompilationError::INVALID_PRINT_TYPE = "Can't print not Int expression";
const std::string CCompilationError::INVALID_CONDITION_TYPE = "Condition should have boolean arguments";
const std::string CCompilationError::INVALID_INDEX_TYPE = "Index should have integer type";
const std::string CCompilationError::CYCLIC_INHERITANCE = "Cyclic inheritance";
const std::string CCompilationError::PARENT_CLASS_NOT_EXISTS = "Parent class does not exist";

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
