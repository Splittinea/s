#include <iostream>
#include <string>
#include "tokenizer.h"
#include "parser.h"
#include "vm.h"

int main() {
    // ===================
    // S LANGUAGE SCRIPT
    // ===================
    std::string code = R"(
        print("Hello");
        print("World");
        print("Heresie");print("Oui");
    )";
    // ===================
    // ==///////////////==
    // ===================

    Tokenizer tokenizer(code);
    auto tokens = tokenizer.tokenize();

    Parser parser(tokens);
    auto program = parser.parseProgram();

    VM vm;
    for (Node* stmt : program) {
        vm.execute(stmt);
    }

    return 0;
}