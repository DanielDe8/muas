#ifndef __PARSER_HPP
#define __PARSER_HPP

#include <string>
#include <vector>
#include <format>
#include "lexer.hpp"

namespace Parser {
    class Node {
        public:
            virtual char size() = 0;

            virtual std::string to_s() = 0;
    };
    class Instruction : public Node {
        public:
            char size() { return 1; }

            std::string mnemonic;
            char r1;
            char r2;

            std::string to_s() override {
                return std::format("Instruction({} [{}], [{}])", mnemonic, r1, r2);
            }

            Instruction(std::string mnemonic, char r1, char r2) {
                this->mnemonic = mnemonic;
                this->r1 = r1;
                this->r2 = r2;
            }
    };
    class Data : public Node {
        public:
            char size() { return size_arg; }

            std::string value;
            Lexer::TokenType type;

            std::string to_s() override {
                return std::format("Data({} type {})", value, static_cast<int>(type));
            }

            char size_arg;

            Data(std::string value, Lexer::TokenType type, char size) {
                this->value = value;
                this->type = type;
                this->size_arg = size;
            }
    };
    class Label : public Node {
        public:
            char size() { return 0; }

            std::string name;

            std::string to_s() override {
                return std::format("Label({})", name);
            }

            Label(std::string name) {
                this->name = name;
            }
    };
    class LabelReference : public Node {
        public:
            char size() { return 4; }

            std::string name;

            std::string to_s() override {
                return std::format("LabelRef({})", name);
            }

            LabelReference(std::string name) {
                this->name = name;
            }
    };
    class Origin : public Node {
        public:
            char size() { return 0; }

            int value;

            std::string to_s() override {
                return std::format("Origin({})", value);
            }

            Origin(int value) {
                this->value = value;
            }
    };

    void parse(Lexer::Token* tokens, std::vector<std::unique_ptr<Node>>* result);
}


#endif