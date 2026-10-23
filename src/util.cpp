#include "util.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <utility>
#include <algorithm>

namespace Util {
    char escape_char(const char ch) {
        switch (ch) {
            case 'n':
                return '\n';
            case 't':
                return '\t';
            case 'r':
                return '\r';
            case 'b':
                return '\b';
            default:
                Util::error("invalid escapee");
                return 1;
        }
    }

    uint8_t reg_to_i(const char r) {
        if (r < 'a' || r > 'd') {
            Util::error("THIS SHOULD NEVER HAPPEN: wrong register char");
            return 0;
        }

        return (uint8_t) (r - 'a');
    }

    std::string ch_to_s(const char ch) {
        std::string s(1, ch);
        return s;
    }

    std::string s_lower(std::string* s) {
        std::string out = *s;
        std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });
        return out;
    }
}
