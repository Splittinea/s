#include <iostream>

// === CUSTOM LIBS ===
#include "vm.h"
#include "ast/ast.h"
#include "core/memory.h"
#include "core/libs/builtins.h"

void VM::execute(Node* node) {
    if (auto printNode = dynamic_cast<PrintNode*>(node)) {
        Builtins::print(printNode->expr, memory);
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
        else if (auto idNode = dynamic_cast<IdentifierNode*>(assignNode->expr)) {
            try {
                Variable v = memory.get(idNode->name);
                if (v.type == Variable::NUM)
                    memory.assignNum(assignNode->name, v.numVal);
                else
                    memory.assignStr(assignNode->name, v.strVal);
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
        else
            std::cerr << "[VM - RED FLAG] Unknown expression type in assignment\n";
    }
    else if (auto builtinNode = dynamic_cast<BuiltinNode*>(node)) {
        if (builtinNode->name == "print") {
            for (auto arg : builtinNode->args)
                Builtins::print(arg, memory);
        }
        else if (builtinNode->name == "input") {
            if (!builtinNode->args.empty()) {
                Node* promptNode = builtinNode->args[0];
                std::string prompt;
                if (auto s = dynamic_cast<StringNode*>(promptNode))
                    prompt = s->value;
                Node* result = Builtins::input(prompt);
                // Optional : Store value in a variable
            } else {
                Builtins::input("");
            }
        }
        else {
            std::cerr << "[VM - RED FLAG] Unknown builtin function: " << builtinNode->name << "\n";
        }
    }
    else {
        std::cerr << "[VM - RED FLAG] Unsupported Instruction\n";
    }
}

void VM::run(const std::vector<Node*>& program) {
    for (auto node : program)
        execute(node);
}