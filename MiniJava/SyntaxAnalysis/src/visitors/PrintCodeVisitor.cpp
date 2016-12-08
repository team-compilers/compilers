#include <PrintCodeVisitor.h>

using namespace AstTree;

void CPrintCodeVisitor::increaseCodeMargin() {
    codeMargin += "    ";
}

void CPrintCodeVisitor::decreaseCodeMargin() {
    codeMargin.erase( codeMargin.size() - 4 );
}

std::string CPrintCodeVisitor::GetCode() const {
    return sstream.str();
}

/*__________ Access Modifiers __________*/

void CPrintCodeVisitor::Visit( const CPublicAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PUBLIC );
    onNodeEnter( nodeName );
    sstream << "public ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::ACCESS_MOD_PRIVATE );
    onNodeEnter( nodeName );
    sstream << "private ";

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CPrintCodeVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BINARY );
    onNodeEnter( nodeName );

    expression->LeftOperand()->Accept( this );
    sstream << ' ' << operatorChar( expression->Operation() ) << ' ';
    expression->RightOperand()->Accept( this );

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_BRACKET );
    onNodeEnter( nodeName );

    expression->ContainerExpression()->Accept( this );
    sstream << '[';
    expression->IndexExpression()->Accept( this );
    sstream << ']';

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NUMBER );
    onNodeEnter( nodeName );

    sstream << expression->Value();

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LOGIC );
    onNodeEnter( nodeName );

    sstream << (expression->Value() ? "true" : "false");

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_ID );
    onNodeEnter( nodeName );

    sstream << expression->Name();

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LENGTH );
    onNodeEnter( nodeName );

    expression->LengthTarget()->Accept( this );
    sstream << ".length";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_METHOD );
    onNodeEnter( nodeName );

    expression->CallerExpression()->Accept( this );
    sstream << '.';

    expression->MethodId()->Accept( this );
    sstream << '(';

    expression->Arguments()->Accept( this );
    sstream << ')';

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CThisExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_THIS );
    onNodeEnter( nodeName );

    sstream << "this";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ARRAY );
    onNodeEnter( nodeName );

    sstream << "new int[";
    expression->LengthExpression()->Accept( this );
    sstream << "]";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEW_ID );
    onNodeEnter( nodeName );

    sstream << "new ";
    expression->TargetId()->Accept( this );
    sstream << "()";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_NEGATE );
    onNodeEnter( nodeName );

    sstream << "!(";
    expression->TargetExpression()->Accept( this );
    sstream << ')';

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CPrintCodeVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID );
    onNodeEnter( nodeName );

    sstream << codeMargin;
    statement->LeftPart()->Accept( this );
    sstream << " = ";
    statement->RightPart()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_ASSIGN_ID_WITH_INDEX );
    onNodeEnter( nodeName );

    sstream << codeMargin;
    statement->LeftPartId()->Accept( this );
    sstream << '[';
    statement->LeftPartIndex()->Accept( this );
    sstream << ']' << " = ";
    statement->RightPart()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CPrintStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_PRINT );
    onNodeEnter( nodeName );

    sstream << codeMargin << "System.out.println(";
    statement->PrintTarget()->Accept( this );
    sstream << ')' << ';' << std::endl;

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_CONDITIONAL );
    onNodeEnter( nodeName );

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

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CWhileLoopStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_WHILE_LOOP );
    onNodeEnter( nodeName );

    sstream << codeMargin << "while (";
    statement->Condition()->Accept( this );
    sstream << ") " << std::endl;

    increaseCodeMargin();
    statement->Body()->Accept( this );
    decreaseCodeMargin();

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CBracesStatement* statement ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_BRACES );
    onNodeEnter( nodeName );

    sstream << codeMargin << '{' << std::endl;
    increaseCodeMargin();
    statement->List()->Accept( this );
    decreaseCodeMargin();
    sstream << codeMargin << '}' << std::endl;

    onNodeExit( nodeName );
}

/*__________ Type Modifiers __________*/

void CPrintCodeVisitor::Visit( const CIntTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT );
    onNodeEnter( nodeName );
    
    sstream << "int ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_BOOL );
    onNodeEnter( nodeName );

    sstream << "boolean ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_INT_ARRAY );
    onNodeEnter( nodeName );

    sstream << "int[] ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( CAstNodeNames::TYPE_MOD_ID );
    onNodeEnter( nodeName );

    typeModifier->TypeId()->Accept( this );
    sstream << ' ';

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CPrintCodeVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL );
    onNodeEnter( nodeName );

    sstream << codeMargin;
    declaration->Type()->Accept( this );
    declaration->Id()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG );
    onNodeEnter( nodeName );

    argument->Type()->Accept( this );
    argument->Id()->Accept( this );

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL );
    onNodeEnter( nodeName );

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

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMainClass* mainClass ) {
    std::string nodeName = generateNodeName( CAstNodeNames::MAIN_CLASS );
    onNodeEnter( nodeName );

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

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CClassDeclaration* declaration ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL );
    onNodeEnter( nodeName );

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

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CProgram* program ) {
    std::string nodeName = generateNodeName( CAstNodeNames::PROGRAM );
    onNodeEnter( nodeName );

    program->MainClass()->Accept( this );
    sstream << std::endl;
    program->ClassDeclarations()->Accept( this );

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CPrintCodeVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::EXP_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CExpression> >& expressions = list->Expressions();
    auto itLast = std::prev( expressions.end() );
    for ( auto it = expressions.begin(); it != expressions.end(); ++it ) {
        ( *it )->Accept( this );
        if (it != itLast) {
            sstream << ", ";
        }
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CStatementList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::STAT_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    // must be reversed before being used
    for ( auto rit = statements.rbegin(); rit != statements.rend(); ++rit ) {
        ( *rit )->Accept( this );
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::VAR_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_ARG_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodArgument> >& methodArguments = list->MethodArguments();
    auto itLast = std::prev( methodArguments.end() );
    for ( auto it = methodArguments.begin(); it != methodArguments.end(); ++it ) {
        ( *it )->Accept( this );
        if (it != itLast) {
            sstream << ", ";
        }
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::METH_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        sstream << std::endl;
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( CAstNodeNames::CLASS_DECL_LIST );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}
