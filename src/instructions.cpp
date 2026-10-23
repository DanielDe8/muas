#include "instructions.hpp"

#include <vector>

namespace Instructions {
    std::vector<Instruction> inst_list = {
        { 0x0, "MOV", false, 0 },
        { 0x1, "ADD", false, 0 },
        { 0x2, "SUB", false, 0 },
        { 0x3, "AND", false, 0 },
        { 0x4, "OR", false, 0 },
        { 0x5, "XOR", false, 0 },
        { 0x6, "SHR", false, 0 },
        { 0x7, "IDC", false, 0 },
        { 0x8, "SJM", false, 0 },
        { 0x9, "JE", false, 0 },
        { 0xA, "JG", false, 0 },
        { 0xB, "JL", false, 0 },
        { 0xC, "MST", false, 0 },
        { 0xD, "IO", false, 0 },
        { 0xE, "MSC", false, 0 },
        { 0xF, "IMM", false, 0 },

        { 0x7, "INC", true, 0b00 },
        { 0x7, "DEC", true, 0b01 },
        { 0x7, "ADC", true, 0b10 },
        { 0x7, "SBC", true, 0b11 },

        { 0x8, "JMP", true, 0b00 },
        { 0x8, "JZ", true, 0b01 },
        { 0x8, "JNZ", true, 0b10 },
        { 0x8, "JC", true, 0b11 },

        { 0xC, "RDM", true, 0b00 },
        { 0xC, "WRM", true, 0b01 },
        { 0xC, "POP", true, 0b10 },
        { 0xC, "PUSH", true, 0b11 },

        { 0xD, "IN", true, 0b00 },
        { 0xD, "OUT", true, 0b01 },

        { 0xE, "LDO", true, 0b00 },
        { 0xE, "RND", true, 0b01 },
        { 0xE, "SDW", true, 0b10 },
        { 0xE, "RBT", true, 0b11 },

        { 0xF, "LDI", true, 0b00 },
        { 0xF, "ADI", true, 0b01 },
    };
}
