#include "builtins.h"

namespace Builtins {

    // =============================
    // === STANDARD IO FUNCTIONS ===
    void print(Node* arg, Memory& memory) {
        if (auto n = dynamic_cast<NumberNode*>(arg)) {
            std::cout << n->value << std::endl;
        } 
        else if (auto s = dynamic_cast<StringNode*>(arg)) {
            std::cout << s->value << std::endl;
        } 
        else if (auto id = dynamic_cast<IdentifierNode*>(arg)) {
            try {
                Variable v = memory.get(id->name);
                if (v.type == Variable::NUM) std::cout << v.numVal << std::endl;
                else std::cout << v.strVal << std::endl;
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else {
            std::cout << "[print: expression inconnue]" << std::endl;
        }
    }

    Node* input(const std::string& prompt) {
        std::cout << prompt;
        std::string value;
        std::getline(std::cin, value);
        StringNode* result = new StringNode();
        result->value = value;
        return result;
    }
    // ===///////////////////////===
    // =============================
}