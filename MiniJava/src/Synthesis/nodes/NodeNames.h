#pragma once

#include <string>

namespace Synthesis {

class CNodeNames {
public:
    static const std::string CJUMP;
    static const std::string JUMP;
    static const std::string CALL;
    static const std::string LABEL;
    static const std::string MOVE_REGISTER;
    static const std::string TEMP;
    static const std::string ADD;
    static const std::string ADD_CONST;
    static const std::string SUB;
    static const std::string SUB_CONST;
    static const std::string MULT;
    static const std::string DIV;
    static const std::string LOAD;
    static const std::string STORE;
    static const std::string MOVE_COMMAND;
    static const std::string NULL_EXP;
    static const std::string EXP_STAT;
};

}
