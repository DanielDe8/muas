#include "bin.hpp"

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include "lexer.hpp"
#include "instructions.hpp"

namespace BinGenerator {
    std::unique_ptr<Parser::Node> eat(std::vector<std::unique_ptr<Parser::Node>>* v) {
        std::unique_ptr<Parser::Node> first = std::move(v->at(0));
        v->erase(v->begin());
        return first;
    }

    void ch_s(uint16_t& size, uint16_t increment) { // check size
        uint16_t new_size = size + increment;
        if (new_size < size) Util::error("program too big");

        size = new_size;
    }

    uint16_t generate_bin(std::vector<std::unique_ptr<Parser::Node>>* ir, uint8_t* out_bin) {
        uint16_t out_size = 0;

        std::unordered_map<std::string, int> labels;

        printf("1st cycle\n");
        // 1st cycle: read labels
        unsigned short pc = 0;
        unsigned short origin = 0;
        for (auto&& node : *ir) {
            if (dynamic_cast<Parser::Label*>(node.get()) != nullptr) {
                labels.insert({ static_cast<Parser::Label*>(node.get())->name, pc + origin });
                printf("label %s at %d + %d = %d\n", static_cast<Parser::Label*>(node.get())->name.c_str(), pc, origin, pc + origin);
            } else if (dynamic_cast<Parser::Origin*>(node.get()) != nullptr) {
                origin = static_cast<Parser::Origin*>(node.get())->value;
            } else {
                pc += node->size();
                printf("(%s) size %d\n", node->to_s().c_str(), node->size());
            }
        }

        printf("2nd cycle\n");
        // 2nd cycle
        while (!ir->empty()) {
            if (dynamic_cast<Parser::Instruction*>(ir->at(0).get()) != nullptr) {
                std::unique_ptr<Parser::Node> node_ptr = eat(ir);
                Parser::Instruction* inst_ptr = static_cast<Parser::Instruction*>(node_ptr.get());

                Instructions::Instruction inst_type = *std::find_if(
                    Instructions::inst_list.begin(), 
                    Instructions::inst_list.end(), 
                    [inst_ptr](auto& i) {
                        return Util::s_lower(&i.mnemonic) == inst_ptr->mnemonic;
                    }
                );

                uint8_t r1_in = inst_ptr->r1;
                uint8_t r2_in = inst_ptr->r2;
                uint8_t r1_out = 0;
                uint8_t r2_out = 0;

                if (r1_in == ' ') r1_in = 'a';
                if (r2_in == ' ') r2_in = 'a';

                if (inst_type.is_sub) {
                    r1_out = inst_type.sub_opcode;
                    r2_out = Util::reg_to_i(r1_in);
                } else {
                    r1_out = Util::reg_to_i(r1_in);
                    r2_out = Util::reg_to_i(r2_in);
                }

                uint8_t bin_line = (inst_type.opcode << 4) + (r1_out << 2) + r2_out;

                ch_s(out_size, 1);
                *out_bin++ = bin_line;
            } else if (dynamic_cast<Parser::Data*>(ir->at(0).get()) != nullptr) {
                std::unique_ptr<Parser::Node> node_ptr = eat(ir);
                Parser::Data* data_ptr = static_cast<Parser::Data*>(node_ptr.get());

                switch (data_ptr->type) {
                    case Lexer::NumLiteral: {
                        int value = std::stoi(data_ptr->value, nullptr, 0);
                        if (data_ptr->size() == 1) { // db
                            ch_s(out_size, 1);
                            *out_bin++ = (uint8_t) value;
                        } else if (data_ptr->size() == 2) { // dw
                            ch_s(out_size, 2);
                            *out_bin++ = (uint8_t) ((value & 0xff00) >> 8); // high byte
                            *out_bin++ = (uint8_t) (value & 0xff); // low byte
                        } else {
                            Util::error("THIS SHOULD NEVER HAPPEN: invalid data size (%d)", data_ptr->size());
                        }
                        break;
                    }
                    case Lexer::CharLiteral: {
                        ch_s(out_size, 1);
                        *out_bin++ = (uint8_t) (data_ptr->value.at(0));
                        break;
                    }
                    case Lexer::StringLiteral: {
                        for (char ch : data_ptr->value) {
                            ch_s(out_size, 1);
                            *out_bin++ = (uint8_t) ch;
                        }
                        break;
                    }
                }
            } else if (dynamic_cast<Parser::LabelReference*>(ir->at(0).get()) != nullptr) {
                std::unique_ptr<Parser::Node> node_ptr = eat(ir);
                Parser::LabelReference* label_ptr = static_cast<Parser::LabelReference*>(node_ptr.get());

                if (!labels.contains(label_ptr->name)) Util::error("label not found: %s", label_ptr->name.c_str());
                uint16_t address = labels.at(label_ptr->name);

                ch_s(out_size, 4);
                *out_bin++ = (uint8_t) 0b11110010;
                *out_bin++ = (uint8_t) ((address & 0xff00) >> 8);
                *out_bin++ = (uint8_t) 0b11110011;
                *out_bin++ = (uint8_t) (address & 0xff);
            } else {
                eat(ir);
            }
        }

        return out_size;
    }
}