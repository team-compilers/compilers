#pragma once

#include <string>

namespace IRTree {

class CTemp {
public:
    CTemp() : name( "T" + std::to_string( counter++ ) ) {}
    CTemp( std::string _name ) : name( _name ) {}

    std::string ToString() { return name; }
private:
    std::string name;
    static int counter;
};

}
