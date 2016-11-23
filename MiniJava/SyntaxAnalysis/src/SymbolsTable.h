// Author: Alexey Zhuravlev

#pragma once

#include <unordered_map>
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
    ClassId,

// Value to return if no valid variable type found
    NotFound
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

// Variable definition 
class CVariableDefinition {
public:
    CVariableDeclaration( CTypeIdentifier _type, const std::string& _name ) 
        : type( _type ), name( _name ) {}

    CTypeIdentifier Type() const { return type; }
    const std::string& Name() const { return name; }
private:
    CTypeIdentifier type;
    std::string name;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class CSymbolsTable {
public:
    CSymbolsTable() {}

    // Add class definition. Return true on success. False if some error occured
    bool AddClassDefinition( const std::string& name, const CClassDefinition* classDefinition );
    // Get class definition by name. Returns zero pointer if it doesn't exist
    const CClassDefinition* GetClassDefinition( const std::string& name ) const;

private:
    std::unordered_map<std::string, std::unique_ptr<CClassDefinition>> classes;
};

////////////////////////////////////////////////////////////////////////////////

class CClassDefinition {
public:
    typedef std::unordered_map<std::string, std::unique_ptr<CMethodDefinition> > TNameToMethodDefinitionMap;
    typedef std::unordered_map<std::string, std::unique_ptr<CFieldDefinition> > TNameToFieldDefinitionMap;

    // Create class defintion without parent
    CClassDefinition( const std::string& _className, const TNameToMethodDefinitionMap& _methods, 
            const TNameToFieldDefinitionMap& _fields )
        : className( _className ), hasParent( false ), methods( _methods ), fields( _fields ) {}

    CClassDefinition( const std::string& _className, const std::string& _parentName,
            const TNameToMethodDefinitionMap& _methods, const TNameToFieldDefinitionMap& _fields )
        : className( _className ), parentName( _parentName ), hasParent( true ), 
          methods( _methods ), fields( _fields ) {}

    // Get method definition by name. Zero if not exists
    const CMethodDefinition* GetMethodDefinition( const std::string& name ) const;
    // Get field definition by name. Zero if not exists
    const CFieldDefinition* GetFieldDefinition( const std::string& name ) const;
private:
    std::string className;
    TNameToFieldDefinitionMap methods;
    TNameToMethodDefinitionMap fields;
    bool hasParent;
    std::string parentName;
};

////////////////////////////////////////////////////////////////////////////////////

class CMethodDefinition {
public:
    
    CMethodDefinition( TAccessModifier _accessModifier, const std::string& methodName, 
            CTypeIdentifier _returnType, std::vector<CVariableDefinition> arguments  );

    // Adds local variable. Returns false on conflict
    bool AddLocalVariable( const std::string& name, CTypeIdentifier type )

    TAccessModifier AccessModifier() const { return accessModifier; }
    const std::string& MethodName() const { return methodName; }
    CTypeIdentifier ReturnType() const { return returnType; }

// Return NotFound in CTypeIdentifier if variable not found
    CTypeIdentifier GetLocalVariableType( const std::string& name ) const;
    CTypeIdentifier GetArgumentType( const std::string& name ) const;

private:
    TAccessModifierType accessModifier;
    std::string methodName;
    CTypeIdentifier returnType;
    std::unordered_map<std::string, CTypeIdentifier> argumentTypes;
    std::unordered_map<std::string, CTypeIdentifier> localVariableTypes;
};

///////////////////////////////////////////////////////////////////////////////////////////

class CFieldDefinition : public CVariableDefinition {
public:
    CFieldDefinition( CTypeIdentifier type, std::string name, TAccessModifier _accessModifier )
        : CVariableDefinition( type, name ), _accessModifier( accessModifier ) {}

    TAccessModifier AccessModifier() const { return accessModifier; }
private:
    TAccessModifier accessModifier;
};
