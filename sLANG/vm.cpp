#include "vm.h"
#include "ast.h"

void VM::execute(Node* node) {
    if (auto printNode = dynamic_cast<PrintNode*>(node)) {
        executePrint(printNode);
    }
    else {
        std::cerr << "[VM - RED FLAG] Unsupported Instruction\n";
    }
}

void VM::executePrint(PrintNode* node) {
    if (auto numNode = dynamic_cast<NumberNode*>(node->expr)) {
        std::cout << numNode->value;
    }
    else if (auto strNode = dynamic_cast<StringNode*>(node->expr)) {
        std::cout << strNode->value;
    }
    else {
        std::cout << "[print: expression inconnue]";
    }
    std::cout << std::endl;
}
