// Author: Alexey Zhuravlev
// Description: Visitor interface

class IVisitor {
    public:
        virtual void Visit( CBinaryExpression* expression ) = 0;
        virtual void Visit( CNumberExpression* expression ) = 0;
}
