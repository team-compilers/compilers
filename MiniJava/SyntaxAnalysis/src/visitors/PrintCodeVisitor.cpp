#include <PrintCodeVisitor.h>

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
    std::string nodeName = generateNodeName( "AccessModPublic" );
    onNodeEnter( nodeName );
    sstream << "public ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CPrivateAccessModifier* modifier ) {
    std::string nodeName = generateNodeName( "AccessModPrivate" );
    onNodeEnter( nodeName );
    sstream << "private ";

    onNodeExit( nodeName );
}

/*__________ Expressions __________*/

void CPrintCodeVisitor::Visit( const CBinaryExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpBinary" );
    onNodeEnter( nodeName );

    expression->LeftOperand()->Accept( this );
    sstream << ' ' << operatorChar( expression->Operation() ) << ' ';
    expression->RightOperand()->Accept( this );

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CBracketExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpBracket" );
    onNodeEnter( nodeName );

    expression->ContainerExpression()->Accept( this );
    sstream << '[';
    expression->IndexExpression()->Accept( this );
    sstream << ']';

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNumberExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNumber" );
    onNodeEnter( nodeName );

    sstream << expression->Value();

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CLogicExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpLogic" );
    onNodeEnter( nodeName );

    sstream << (expression->Value() ? "true" : "false");

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CIdExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpId" );
    onNodeEnter( nodeName );

    sstream << expression->Name();

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CLengthExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpLength" );
    onNodeEnter( nodeName );

    expression->LengthTarget()->Accept( this );
    sstream << ".length";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpMethod" );
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
    std::string nodeName = generateNodeName( "ExpThis" );
    onNodeEnter( nodeName );

    sstream << "this";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNewArrayExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNewArray" );
    onNodeEnter( nodeName );

    sstream << "new int[";
    expression->LengthExpression()->Accept( this );
    sstream << "]";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNewIdExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNewId" );
    onNodeEnter( nodeName );

    sstream << "new ";
    expression->TargetId()->Accept( this );
    sstream << "()";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CNegateExpression* expression ) {
    std::string nodeName = generateNodeName( "ExpNegate" );
    onNodeEnter( nodeName );

    sstream << "!(";
    expression->TargetExpression()->Accept( this );
    sstream << ')';

    onNodeExit( nodeName );
}

/*__________ Statements __________*/

void CPrintCodeVisitor::Visit( const CAssignIdStatement* statement ) {
    std::string nodeName = generateNodeName( "StatAssignId" );
    onNodeEnter( nodeName );

    sstream << codeMargin;
    statement->LeftPart()->Accept( this );
    sstream << " = ";
    statement->RightPart()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CAssignIdWithIndexStatement* statement ) {
    std::string nodeName = generateNodeName( "StatAssignIdWithIndex" );
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
    std::string nodeName = generateNodeName( "StatPrint" );
    onNodeEnter( nodeName );

    sstream << codeMargin << "System.out.println(";
    statement->PrintTarget()->Accept( this );
    sstream << ')' << ';' << std::endl;

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CConditionalStatement* statement ) {
    std::string nodeName = generateNodeName( "StatConditional" );
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
    std::string nodeName = generateNodeName( "StatWhileLoop" );
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
    std::string nodeName = generateNodeName( "StatBraces" );
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
    std::string nodeName = generateNodeName( "TypeModInt" );
    onNodeEnter( nodeName );
    
    sstream << "int ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CBooleanTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModBool" );
    onNodeEnter( nodeName );

    sstream << "boolean ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CIntArrayTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModIntArray" );
    onNodeEnter( nodeName );

    sstream << "int[] ";

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CIdTypeModifier* typeModifier ) {
    std::string nodeName = generateNodeName( "TypeModId" );
    onNodeEnter( nodeName );

    typeModifier->TypeId()->Accept( this );
    sstream << ' ';

    onNodeExit( nodeName );
}

/*__________ Other (except lists) __________*/

void CPrintCodeVisitor::Visit( const CVarDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "VarDecl" );
    onNodeEnter( nodeName );

    sstream << codeMargin;
    declaration->Type()->Accept( this );
    declaration->Id()->Accept( this );
    sstream << ';' << std::endl;

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodArgument* argument ) {
    std::string nodeName = generateNodeName( "MethArg" );
    onNodeEnter( nodeName );

    argument->Type()->Accept( this );
    argument->Id()->Accept( this );

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodDeclaration* declaration ) {
    std::string nodeName = generateNodeName( "MethDecl" );
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
    std::string nodeName = generateNodeName( "MainClass" );
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
    std::string nodeName = generateNodeName( "ClassDecl" );
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
    std::string nodeName = generateNodeName( "Program" );
    onNodeEnter( nodeName );

    program->MainClass()->Accept( this );
    sstream << std::endl;
    program->ClassDeclarations()->Accept( this );

    onNodeExit( nodeName );
}

/*__________  Lists __________*/

void CPrintCodeVisitor::Visit( const CExpressionList* list ) {
    std::string nodeName = generateNodeName( "ExpList" );
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
    std::string nodeName = generateNodeName( "StatList" );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CStatement> >& statements = list->Statements();
    // must be reversed before being used
    for ( auto rit = statements.rbegin(); rit != statements.rend(); ++rit ) {
        ( *rit )->Accept( this );
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CVarDeclarationList* list ) {
    std::string nodeName = generateNodeName( "VarDeclList" );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CVarDeclaration> >& varDeclarations = list->VarDeclarations();
    for ( auto it = varDeclarations.begin(); it != varDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CMethodArgumentList* list ) {
    std::string nodeName = generateNodeName( "MethArgList" );
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
    std::string nodeName = generateNodeName( "MethDeclList" );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CMethodDeclaration> >& methodDeclarations = list->MethodDeclarations();
    for ( auto it = methodDeclarations.begin(); it != methodDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
        sstream << std::endl;
    }

    onNodeExit( nodeName );
}

void CPrintCodeVisitor::Visit( const CClassDeclarationList* list ) {
    std::string nodeName = generateNodeName( "ClassDeclList" );
    onNodeEnter( nodeName );

    const std::vector< std::unique_ptr<const CClassDeclaration> >& classDeclarations = list->ClassDeclarations();
    for ( auto it = classDeclarations.begin(); it != classDeclarations.end(); ++it ) {
        ( *it )->Accept( this );
    }

    onNodeExit( nodeName );
}
