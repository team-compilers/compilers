#pragma once

#include <string>

namespace ASTree {

class CNodeNames {
public:
    static const std::string ACCESS_MOD_PRIVATE;
    static const std::string ACCESS_MOD_PUBLIC;
    static const std::string CLASS_DECL;
    static const std::string CLASS_DECL_LIST;
    static const std::string EXP_BINARY;
    static const std::string EXP_BRACKET;
    static const std::string EXP_ID;
    static const std::string EXP_LENGTH;
    static const std::string EXP_LIST;
    static const std::string EXP_LOGIC;
    static const std::string EXP_METHOD;
    static const std::string EXP_NEGATE;
    static const std::string EXP_NEW_ARRAY;
    static const std::string EXP_NEW_ID;
    static const std::string EXP_NUMBER;
    static const std::string EXP_THIS;
    static const std::string MAIN_CLASS;
    static const std::string METH_ARG;
    static const std::string METH_ARG_LIST;
    static const std::string METH_DECL;
    static const std::string METH_DECL_LIST;
    static const std::string PROGRAM;
    static const std::string STAT_ASSIGN_ID;
    static const std::string STAT_ASSIGN_ID_WITH_INDEX;
    static const std::string STAT_BRACES;
    static const std::string STAT_CONDITIONAL;
    static const std::string STAT_LIST;
    static const std::string STAT_PRINT;
    static const std::string STAT_WHILE_LOOP;
    static const std::string TYPE_MOD_BOOL;
    static const std::string TYPE_MOD_ID;
    static const std::string TYPE_MOD_INT;
    static const std::string TYPE_MOD_INT_ARRAY;
    static const std::string VAR_DECL;
    static const std::string VAR_DECL_LIST;
};

}
