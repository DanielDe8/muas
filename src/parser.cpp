#include "parser.hpp"

#include <string>
#include <vector>
#include "lexer.hpp"
#include "util.hpp"

namespace Parser {
    char reg_or_empty(Lexer::Token*& tokens) { 
        if (tokens->type == Lexer::Register) { 
            return tokens++->value[0];
        } else {
            return ' ';
        } 
    }
    
    void parse(Lexer::Token* tokens, std::vector<std::unique_ptr<Parser::Node>>* result) {
        while (tokens->type != Lexer::EndOfFile) {
            // printf("type %d val \"%s\"\n", tokens->type, tokens->value.c_str());

            switch (tokens->type) {
                case Lexer::Instruction: {
                    std::string mnemonic = (tokens++)->value;
                    char r1 = reg_or_empty(tokens);
                    char r2 = ' ';

                    if (tokens->type == Lexer::Comma && r1 != ' ') {
                        tokens++; // comma
                        r2 = reg_or_empty(tokens);
                    }

                    result->push_back(std::make_unique<Instruction>(mnemonic, r1, r2));
                    break;
                }
                case Lexer::DefineByte: {
                    tokens++; // db
                    Lexer::Token* next = tokens++;

                    int size = -1;
                    switch (next->type) {
                        case Lexer::NumLiteral: {
                            size = 1;
                            break;
                        }
                        case Lexer::CharLiteral: {
                            size = 1;
                            break;
                        }
                        case Lexer::StringLiteral: {
                            size = next->value.length();
                            break;
                        }
                        default: {
                            Util::error("expected data, found \"%s\"", next->value.c_str());
                        }
                    }

                    result->push_back(std::make_unique<Data>(next->value, next->type, size));
                    break;
                }
                case Lexer::DefineWord: {
                    tokens++;
                    Lexer::Token* next = tokens++;

                    if (next->type != Lexer::NumLiteral) {
                        Util::error("expected numeric data, found \"%s\"", next->value.c_str());
                    }

                    result->push_back(std::make_unique<Data>(next->value, Lexer::NumLiteral, 2));
                    break;
                }
                case Lexer::Label: {
                    result->push_back(std::make_unique<Label>((tokens++)->value));
                    break;
                }
                case Lexer::LabelReference: {
                    result->push_back(std::make_unique<LabelReference>((tokens++)->value));
                    break;
                }
                case Lexer::Origin: {
                    tokens++; // org
                    result->push_back(std::make_unique<Origin>(std::stoi((tokens++)->value, nullptr, 0)));
                    break;
                }
                default: {
                    Util::error("unexpected token: %d(%s)", tokens->type, tokens->value.c_str());
                }
            }
            // printf("ir: %s\n", result->at(result->size()-1)->to_s().c_str());
            // printf("next tk: (type %d val %s)\n\n", tokens->type, tokens->value.c_str());
            // fflush(stdout);
        }
    }
}
