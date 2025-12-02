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
        print("Hello World");

        num x;
        x = 10;
        print(x);

        num y;
        y = 42.5;
        print(y);

        str prenom;
        prenom = "Ugo";
        print(prenom);

        str message;
        message = "Bienvenue dans Splittix!";
        print(message);

        print("Test1"); print("Test2");
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
