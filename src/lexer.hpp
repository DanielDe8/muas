#ifndef __LEXER_HPP
#define __LEXER_HPP

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include "util.hpp"

namespace Lexer {
    enum TokenType {
        DefineByte,
        DefineWord,
        Origin,
        Register,
        Instruction,

        Label,
        LabelReference,

        StringLiteral,
        CharLiteral,
        NumLiteral,

        Comma,

        EndOfFile
    };

    struct Token {
        enum TokenType type;
        std::string value;
    };

    extern std::unordered_map<std::string, enum TokenType> keywords;

    void init();
    void tokenize(char* src, std::vector<Token>* tokens);
}

#endif
