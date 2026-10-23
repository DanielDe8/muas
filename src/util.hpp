#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <utility>

namespace Util {
    const size_t max_label_size = 64;
    const size_t max_string_lit_size = 256;
    const size_t max_num_lit_size = 32;
    const size_t max_identifier_size = 64;
    const size_t max_keyword_size = 8;

    char escape_char(const char ch);
    uint8_t reg_to_i(const char r);
    std::string ch_to_s(const char ch);
    std::string s_lower(std::string* s);

    template <typename... Args>
    [[noreturn]] void error(const char* msg, Args&&... args) {
        char msgf[256];
        snprintf(msgf, sizeof(msgf), msg, std::forward<Args>(args)...);
        fprintf(stderr, "\033[39;49;1mmuas: \033[31;1mERROR: \033[0m%s\n", msgf);
        std::exit(1);
    }
}

#endif