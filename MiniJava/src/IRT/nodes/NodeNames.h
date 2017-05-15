#pragma once

#include <string>

namespace IRTree {

class CNodeNames {
public:
    static const std::string EXP_BINARY;
    static const std::string EXP_CALL;
    static const std::string EXP_CONST;
    static const std::string EXP_ESEQ;
    static const std::string EXP_MEM;
    static const std::string EXP_NAME;
    static const std::string EXP_TEMP;

    static const std::string STAT_EXP;
    static const std::string STAT_CJUMP;
    static const std::string STAT_JUMP;
    static const std::string STAT_LABEL;
    static const std::string STAT_MOVE;
    static const std::string STAT_SEQ;

    static const std::string LIST_EXP;
    static const std::string LIST_STAT;
};

}
