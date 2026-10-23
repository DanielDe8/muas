#ifndef __BIN_HPP
#define __BIN_HPP

#include <cstddef>
#include <vector>
#include <memory>
#include "parser.hpp"

namespace BinGenerator {
    uint16_t generate_bin(std::vector<std::unique_ptr<Parser::Node>>* ir, uint8_t* out_bin);
}

#endif