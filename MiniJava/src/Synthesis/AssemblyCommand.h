#include <string>
#include <vector>

class CAssemblyCommand {
public:
    CAssemblyCommand( const std::string _text, const std::vector<const std::string>& _registers ) :
        text( _text ),
        registers( _registers ) {}
    
private:
    const std::string text;
    const std::vector<const std::string> registers;
};