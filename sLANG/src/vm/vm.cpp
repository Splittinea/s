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
        if (declNode->type == Variable::NUM) {
            if (declNode->setName.empty() || declNode->setName == "R") {
                memory.declareNum(declNode->name, Variable::R);
            }
            else if (declNode->setName == "Z") {
                memory.declareInt(declNode->name);
            }
            /*else if (declNode->setName == "N") {
                memory.declareNat(declNode->name);
            }
            else if (declNode->setName == "Q") {
                memory.declareRational(declNode->name);
            }
            else if (declNode->setName == "C") {
                memory.declareComplex(declNode->name);
            }*/
            else {
                std::cerr << "[VM - RED FLAG] Unknown number set: " << declNode->setName << "\n";
            }
        }
        else {
            // Pour les chaînes de caractères
            memory.declareStr(declNode->name);
        }
    }
    else if (auto assignNode = dynamic_cast<AssignNode*>(node)) {
        if (auto numNode = dynamic_cast<NumberNode*>(assignNode->expr))
            memory.assignNum(assignNode->name, numNode->value);
        else if (auto strNode = dynamic_cast<StringNode*>(assignNode->expr))
            memory.assignStr(assignNode->name, strNode->value);
        else
            std::cerr << "[VM - RED FLAG] Unknown expression type in assignment\n";
    }
    else if (auto builtinNode = dynamic_cast<BuiltinNode*>(node)) {
        if (builtinNode->name == "print") {
            for (auto arg : builtinNode->args)
                Builtins::print(arg, memory);
        }
        else if (builtinNode->name == "input") {
            if (builtinNode->args.size() > 0) {
                Node* result = Builtins::input(dynamic_cast<StringNode*>(builtinNode->args[0])->value);
                // Optionnel : stocker le résultat si assigné
            } else {
                Builtins::input("");
            }
        } else {
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