#include <iostream>
#include <vector>
#include <string>

// CUSTOM LIBS
#include "core/memory.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "vm/vm.h"

int main() {
    // ===== S Language Script =====
    std::string code = R"(
        num(R) a;
        num(Z) b;

        a = 3.14;
        b = -7;

        print(a);
        print(b);
    )";
    // =============================
    
    // === EXECUTION CHAIN ===
    try {
        // ===== Tokenization =====
        Tokenizer tokenizer(code);
        std::vector<Token> tokens = tokenizer.tokenize();

        // ===== Parsing =====
        Parser parser(tokens);
        std::vector<Node*> program = parser.parseProgram();

        // ===== Memory & VM =====
        Memory memory;
        VM vm(memory);
        vm.run(program);

        // ===== Cleanup =====
        for (Node* node : program) {
            delete node;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << std::endl;
    }

    return 0;
}
