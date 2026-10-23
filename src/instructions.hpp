#ifndef __INSTRUCTIONS_HPP
#define __INSTRUCTIONS_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "util.hpp"

namespace Instructions {
    struct Instruction {
        uint8_t opcode;
        std::string mnemonic;
        bool is_sub;
        uint8_t sub_opcode;
    };

    extern std::vector<Instruction> inst_list;
}

#endif
