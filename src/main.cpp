#include <cstdio>
#include <cstddef>
#include <bitset>
#include "lexer.hpp"
#include "parser.hpp"
#include "bin.hpp"
#include "util.hpp"

using namespace std;

int main() {
    // char src[] = R"(
    //     org 0x100

    //     mov a,b
    //     .start
    //     db 3
    //     db 0b10101
    //     dw 0xdeadbeef
    //     :start
    //     jmp
    // )";
    char src[] = R"(
        :data
        push c
        push d
        
        .loop
            pop d
            pop c

            rdm a
            
            inc d
            adc c

            push c
            push d

            :end
            jz a

            out a
            db 0

            :loop
            jmp

        .end
            sdw

        .data
            db "hello"
            db 0
    )";

    Lexer::init();

    std::vector<Lexer::Token> tokens;
    Lexer::tokenize(src, &tokens);

    printf("LEXER %d\n", tokens.size());
    size_t i = 0;
    for (auto token : tokens) {
        printf("%d: type %d val \"%s\"\n", i++, token.type, token.value.c_str());
    }
    printf("\n\n");

    std::vector<std::unique_ptr<Parser::Node>> ir;
    Parser::parse(&tokens[0], &ir);

    printf("IR %d\n", ir.size());
    for (auto&& node : ir) {
        printf("%d: %s\n", i++, node->to_s().c_str());
    }
    printf("\n\n");

    uint8_t* out_bin = (uint8_t*) malloc(sizeof(uint8_t) * 65536);
    uint16_t out_size = BinGenerator::generate_bin(&ir, out_bin);

    printf("OUT %d\n\n", out_size);
    for (i = 0; i < out_size; i++) {
        std::string binary = bitset<8>(out_bin[i]).to_string();
        printf("%s (%c)\n", binary.c_str(), out_bin[i]);
    }

    FILE* out_file = fopen("program.bin", "w");

    fwrite(out_bin, 1, out_size, out_file);

    fclose(out_file);

    return 0;
}
