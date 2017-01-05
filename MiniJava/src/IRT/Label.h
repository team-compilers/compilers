#pragma once

#include <string>

namespace IRTree {

class CLabel {
public:
    CLabel() : label( "L" + std::to_string( counter++ ) ) {};
    CLabel( std::string _label ) : label( _label ) {}

    std::string ToString() { return label; }
private:
    std::string label;
    static int counter;
};

}
