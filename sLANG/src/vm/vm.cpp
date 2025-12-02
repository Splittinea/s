#include "vm.h"
#include "ast/ast.h"
#include "core/memory.h"

#include <iostream>

void VM::execute(Node* node) {
    if (auto printNode = dynamic_cast<PrintNode*>(node)) {
        sPrint(printNode);
    }
    else if (auto declNode = dynamic_cast<DeclNode*>(node)) {
        if (declNode->type == Variable::NUM)
            memory.declareNum(declNode->name);
        else
            memory.declareStr(declNode->name);
    }
    else if (auto assignNode = dynamic_cast<AssignNode*>(node)) {
        if (auto numNode = dynamic_cast<NumberNode*>(assignNode->expr))
            memory.assignNum(assignNode->name, numNode->value);
        else if (auto strNode = dynamic_cast<StringNode*>(assignNode->expr))
            memory.assignStr(assignNode->name, strNode->value);
        else
            std::cerr << "[VM - RED FLAG] Unknown expression type in assignment\n";
    }
    else {
        std::cerr << "[VM - RED FLAG] Unsupported Instruction\n";
    }
}

void VM::sPrint(PrintNode* node) {
    if (auto numNode = dynamic_cast<NumberNode*>(node->expr))
        std::cout << numNode->value;
    else if (auto strNode = dynamic_cast<StringNode*>(node->expr))
        std::cout << strNode->value;
    else if (auto idNode = dynamic_cast<IdentifierNode*>(node->expr)) {
        try {
            Variable v = memory.get(idNode->name);
            if (v.type == Variable::NUM) std::cout << v.numVal;
            else std::cout << v.strVal;
        } catch (std::exception& e) {
            std::cerr << e.what();
        }
    }
    else
        std::cout << "[print: expression inconnue]";

    std::cout << std::endl;
}

void VM::run(const std::vector<Node*>& program) {
    for (auto node : program)
        execute(node);
}