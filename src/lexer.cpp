#include "lexer.hpp"

#include <ctype.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include "instructions.hpp"
#include "util.hpp"

namespace Lexer {
    std::unordered_map<std::string, enum TokenType> keywords = {
        { "db", DefineByte },
        { "dw", DefineWord },
        { "org", Origin },

        { "a", Register },
        { "b", Register },
        { "c", Register },
        { "d", Register },
    };

    void init() {
        for (int i = 0; i < Instructions::inst_list.size(); i++) {
            Instructions::Instruction inst = Instructions::inst_list.at(i);

            keywords.insert({ Util::s_lower(&inst.mnemonic), Instruction });
        }
    }

    void tokenize(char* src, std::vector<Token>* tokens) {
        while (*src) {
            switch (*src) {
                case ',': {
                    tokens->push_back({ Comma, Util::ch_to_s(*src++) });
                    break;
                }
                case '.': {
                    src++;

                    std::string label;

                    while (*src && (isalpha(*src) || *src == '_')) {
                        if (label.length() == Util::max_label_size) {
                            Util::error("label name too long");
                        }
                        label += *src++;
                    }

                    tokens->push_back({ Label, label });
                    break;
                }
                case ':': {
                    src++;

                    std::string ref;

                    while (*src && (isalpha(*src) || *src == '_')) {
                        if (ref.length() == Util::max_label_size) {
                            Util::error("label name (reference) too long");
                        }
                        ref += *src++;
                    }

                    tokens->push_back({ LabelReference, ref });
                    break;
                }
                case '\'': {
                    src++;

                    std::string char_lit(1, *src++);

                    if (char_lit[0] == '\\') {
                        char_lit[0] = Util::escape_char(*src++);
                    }

                    tokens->push_back({ CharLiteral, char_lit });

                    if (*src++ != '\'') {
                        Util::error("incorrect character literal");
                    }

                    break;
                }
                case '"': {
                    src++;

                    std::string string_lit;

                    while (*src && *src != '"' && string_lit[0] != '\\') {
                        string_lit += *src++;
                    }

                    if (!*src) {
                        Util::error("missing closing quote");
                    }
                    src++; // "

                    tokens->push_back({ StringLiteral, string_lit });
                    break;
                }
                case ';': {
                    while (*src && *src != '\n') {
                        src++;
                    }
                }
                default: {
                    if (isdigit(*src)) {
                        std::string num_lit;

                        while (
                            *src && (isdigit(*src) || (num_lit.length() == 1 && (*src == 'b' || *src == 'x')) || (num_lit.length() > 1 && (
                                *src == 'a' ||
                                *src == 'b' ||
                                *src == 'c' ||
                                *src == 'd' ||
                                *src == 'e' ||
                                *src == 'f'
                            )))
                        ) {
                            if (num_lit.length() == Util::max_num_lit_size) {
                                Util::error("numeric literal too long");
                            }

                            num_lit += *src++;
                        }

                        tokens->push_back({ NumLiteral, num_lit });
                    } else if (isalpha(*src)) {
                        std::string identifier;

                        while (*src && isalpha(*src)) {
                            if (identifier.length() == Util::max_identifier_size) {
                                Util::error("identifier too long");
                            }

                            identifier += *src++;
                        }

                        if (keywords.count(identifier) == 0) {
                            Util::error("unresolved reference: %s", identifier.c_str());
                        }
                        tokens->push_back({ keywords[identifier], identifier });
                    } else if (src[0] == ' ' || src[0] == '\n' || src[0] == '\t') {
                        src++;
                    } else {
                        char ch = src[0];
                        Util::error("unexpected character: %c", ch);
                    }
                }
            }   
        }

        tokens->push_back({ EndOfFile, "" });
    }
}
