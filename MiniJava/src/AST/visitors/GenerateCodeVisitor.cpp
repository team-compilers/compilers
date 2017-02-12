#include <AST/visitors/GenerateCodeVisitor.h>

using namespace ASTree;

void CGenerateCodeVisitor::increaseCodeMargin() {
    codeMargin += "    ";
}

void CGenerateCodeVisitor::decreaseCodeMargin() {
    codeMargin.erase( codeMargin.size() - 4 );
}

std::string CGenerateCodeVisitor::GetCode() const {
    return sstream.str();
}

/*__________ Access Modifiers __________*/

void CGenerateCodeVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName, modifier->Location() );
    sstream << "public ";

    onNodeExit( nodeName, modifier->Location() );
}

void CGenerateCodeVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName, modifier->Location() );
    sstream << "private ";

    onNodeExit( nodeName, modifier->Location() );
}

/*__________ Expressions __________*/

void CGenerateCodeVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BINARY );
    onNodeEnter( nodeName, expression->Location() );

    expression->LeftOperand()->Accept( this );
    sstream << ' ' << operatorChar( expression->Operation() ) << ' ';
    expression->RightOperand()->Accept( this );

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName, expression->Location() );

    expression->ContainerExpression()->Accept( this );
    sstream << '[';
    expression->IndexExpression()->Accept( this );
    sstream << ']';

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName, expression->Location() );

    sstream << expression->Value();

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName, expression->Location() );

    sstream << (expression->Value() ? "true" : "false");

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_ID );
    onNodeEnter( nodeName, expression->Location() );

    sstream << expression->Name();

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName, expression->Location() );

    expression->LengthTarget()->Accept( this );
    sstream << ".length";

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_METHOD );
    onNodeEnter( nodeName, expression->Location() );

    expression->CallerExpression()->Accept( this );
    sstream << '.';

    expression->MethodId()->Accept( this );
    sstream << '(';

    expression->Arguments()->Accept( this );
    sstream << ')';

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_THIS );
    onNodeEnter( nodeName, expression->Location() );

    sstream << "this";

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName, expression->Location() );

    sstream << "new int[";
    expression->LengthExpression()->Accept( this );
    sstream << "]";

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName, expression->Location() );

    sstream << "new ";
    expression->TargetId()->Accept( this );
    sstream << "()";

    onNodeExit( nodeName, expression->Location() );
}

void CGenerateCodeVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName, expression->Location() );

    sstream << "!(";
    expression->TargetExpression()->Accept( this );
    sstream << ')';

    onNodeExit( nodeName, expression->Location() );
}

/*__________ Statements __________*/

void CGenerateCodeVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName, statement->Location() );

    sstream << codeMargin;
    statement->LeftPart()->Accept( this );
    sstream << " = ";
    statement->RightPart()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName, statement->Location() );
}

void CGenerateCodeVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName, statement->Location() );

    sstream << codeMargin;
    statement->LeftPartId()->Accept( this );
    sstream << '[';
    statement->LeftPartIndex()->Accept( this );
    sstream << ']' << " = ";
    statement->RightPart()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName, statement->Location() );
}

void CGenerateCodeVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_PRINT );
    onNodeEnter( nodeName, statement->Location() );

    sstream << codeMargin << "System.out.println(";
    statement->PrintTarget()->Accept( this );
    sstream << ')' << ';' << std::endl;

    onNodeExit( nodeName, statement->Location() );
}

void CGenerateCodeVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName, statement->Location() );

    sstream << codeMargin << "if (";
    statement->Condition()->Accept( this );
    sstream << ") " << std::endl;
    increaseCodeMargin();
    statement->PositiveTarget()->Accept( this );
    decreaseCodeMargin();
    sstream << codeMargin << "else " << std::endl;
    increaseCodeMargin();
    statement->NegativeTarget()->Accept( this );
    decreaseCodeMargin();

    onNodeExit( nodeName, statement->Location() );
}

void CGenerateCodeVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName, statement->Location() );

    sstream << codeMargin << "while (";
    statement->Condition()->Accept( this );
    sstream << ") " << std::endl;

    increaseCodeMargin();
    statement->Body()->Accept( this );
    decreaseCodeMargin();

    onNodeExit( nodeName, statement->Location() );
}

void CGenerateCodeVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_BRACES );
    onNodeEnter( nodeName, statement->Location() );

    sstream << codeMargin << '{' << std::endl;
    increaseCodeMargin();
    statement->List()->Accept( this );
    decreaseCodeMargin();
    sstream << codeMargin << '}' << std::endl;

    onNodeExit( nodeName, statement->Location() );
}

/*__________ Type Modifiers __________*/

void CGenerateCodeVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName, typeModifier->Location() );
    
    sstream << "int ";

    onNodeExit( nodeName, typeModifier->Location() );
}

void CGenerateCodeVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName, typeModifier->Location() );

    sstream << "boolean ";

    onNodeExit( nodeName, typeModifier->Location() );
}

void CGenerateCodeVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName, typeModifier->Location() );

    sstream << "int[] ";

    onNodeExit( nodeName, typeModifier->Location() );
}

void CGenerateCodeVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName, typeModifier->Location() );

    typeModifier->TypeId()->Accept( this );
    sstream << ' ';

    onNodeExit( nodeName, typeModifier->Location() );
}

/*__________ Other (except lists) __________*/

void CGenerateCodeVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    sstream << codeMargin;
    declaration->Type()->Accept( this );
    declaration->Id()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName, declaration->Location() );
}

void CGenerateCodeVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG );
    onNodeEnter( nodeName, argument->Location() );

    argument->Type()->Accept( this );
    argument->Id()->Accept( this );

    onNodeExit( nodeName, argument->Location() );
}

void CGenerateCodeVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    sstream << codeMargin;
    declaration->AccessModifier()->Accept( this );
    declaration->TypeModifier()->Accept( this );
    declaration->MethodId()->Accept( this );
    sstream << '(';
    declaration->MethodArguments()->Accept( this );
    sstream << ") {" << std::endl;
    increaseCodeMargin();
    declaration->VarDeclarations()->Accept( this );
    declaration->Statements()->Accept( this );
    sstream << codeMargin << "return ";
    declaration->ReturnExpression()->Accept( this );
    sstream << ';' << std::endl;
    decreaseCodeMargin();
    sstream << codeMargin << '}' << std::endl;

    onNodeExit( nodeName, declaration->Location() );
}

void CGenerateCodeVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName, mainClass->Location() );

    sstream << codeMargin << "class ";
    mainClass->ClassName()->Accept( this );
    sstream << ' ';
    sstream << '{' << std::endl;
    increaseCodeMargin();
    sstream << codeMargin << "public static void main(String[] ";
    mainClass->ClassArgsName()->Accept( this );
    sstream << ") {" << std::endl;
    increaseCodeMargin();
    mainClass->Statements()->Accept( this );
    decreaseCodeMargin();
    sstream << codeMargin << '}' << std::endl;
    decreaseCodeMargin();
    sstream << codeMargin << '}' << std::endl;

    onNodeExit( nodeName, mainClass->Location() );
}

void CGenerateCodeVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL );
    onNodeEnter( nodeName, declaration->Location() );

    sstream << codeMargin << "class ";
    declaration->ClassName()->Accept( this );
    sstream << ' ';
    if ( declaration->HasParent() ) {
        sstream << "extends ";
        declaration->ExtendsClassName()->Accept( this );
    }
    sstream << '{' << std::endl;
    increaseCodeMargin();

    declaration->VarDeclarations()->Accept( this );
    declaration->MethodDeclarations()->Accept( this );

    decreaseCodeMargin();
    sstream << codeMargin << '}' << std::endl;

    onNodeExit( nodeName, declaration->Location() );
}

void CGenerateCodeVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CNodeNames::PROGRAM );
    onNodeEnter( nodeName, program->Location() );

    program->MainClass()->Accept( this );
    sstream << std::endl;
    program->ClassDeclarations()->Accept( this );

    onNodeExit( nodeName, program->Location() );
}

/*__________  Lists __________*/

void CGenerateCodeVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::EXP_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CExpression> >& expressions = list->Expressions();
    auto itLast = std::prev( expressions.end() );
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );
        if (it != itLast) {
            sstream << ", ";
        }
    }

    onNodeExit( nodeName, list->Location() );
}

void CGenerateCodeVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::STAT_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    // must be reversed before being used
    for ( auto rit = statements.rbegin(); rit != statements.rend(); ++rit ) {
        ( *rit )->Accept( this );
    }

    onNodeExit( nodeName, list->Location() );
}

void CGenerateCodeVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName, list->Location() );
}

void CGenerateCodeVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    auto itLast = std::prev( methodArguments.end() );
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
        if (it != itLast) {
            sstream << ", ";
        }
    }

    onNodeExit( nodeName, list->Location() );
}

void CGenerateCodeVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        sstream << std::endl;
    }

    onNodeExit( nodeName, list->Location() );
}

void CGenerateCodeVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName, list->Location() );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName, list->Location() );
}
