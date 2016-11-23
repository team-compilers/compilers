// Author: Alexey Zhuravlev
#include <map>
#include <memory>
#include <assert>

// Predeclarations and general enum and classes declarations

class CClassDefinition;
class CMethodDefinition;
class CFieldDefinition;

enum class TAccessModifier {
    Public,
    Private
};

enum class TTypeIdentifier {
    Int,
    IntArray,
    Boolean,
    ClassId
};

// Type identifier class for other methods
class CTypeIdentifier {
public:
    CTypeIdentifier( TTypeIdentifier _type )
        : type( _type ) { assert( type != TTypeIdentifier::ClassId ) }

    explicit CTypeIdentifier( const std::string& _className )
        : Type( TTypeIdentifier::ClassId ), className( _className ) {}

    TTypeIdentifier Type() const { return type; }
    const std::string& ClassName() const { return className; }

private:
    TTypeIdentifier type;
    // Valid if Type==TTypeIdentifier::ClassId
    std::string className;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class CSymbolsTable {
public:
    CSymbolsTable() {}

    // Add class definition. Return true on success. False if some error occured
    bool AddClassDefinition( const std::string& name, const CClassDefinition* classDefinition );
    // Get class definition by name. Returns zero pointer if it doesn't exist
    const CClassDefinition* GetClassDefinition( const std::string& name )

private:
    std::map<std::string, std::unique_ptr<CClassDefinition>> classes;
};

////////////////////////////////////////////////////////////////////////////////

class CClassDefinition {
public:
    // Create class defintion without parent
    CClassDefinition( const std::string& _className )
        : className( _className ), hasParent( false ) {}
    CClassDefinition( const std::string& _className, const std::string& _parentName )
        : className( _className ), parentName( _parentName ), hasParent( true ) {}
    // Add method definition. True on success
    bool AddMethodDefinition( const std::string& name, const CMethodDefinition* methodDefinition );
    // Get method definition by name. Zer if not exists
    const CMethodDefinition* GetMethodDefinition( const std::string& name );
    // Add field definition. True on success
    bool AddFieldDefinition( const std::string& name, const CFieldDefinition* fieldDefinition );
    // Get field definition by name. Zero if not exists
    const CFieldDefinition* GetFieldDefinition( const std::string& name );
private:
    std::string className;
    std::map<std::string, std::unique_ptr<CMethodDefinition> > methods;
    std::map<std::string, std::unique_ptr<CFieldDefinition> > fields;
    bool hasParent;
    std::string parentName;
};

////////////////////////////////////////////////////////////////////////////////////

// Сигнатура метода
class CMethodSignature {
    
};

class CMethodDefinition {
public:

private:
    TAccessModifierType accessModifierType;
    std::string name;
};
