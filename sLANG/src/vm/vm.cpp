#include <iostream>

// === CUSTOM LIBS ===
#include "vm.h"
#include "ast/ast.h"
#include "core/memory.h"
#include "core/libs/builtins.h"
#include "core/value.h"

void VM::execute(Node* node) {
    if (auto printNode = dynamic_cast<PrintNode*>(node)) {
        Builtins::print(printNode->expr, memory);
    }
    else if (auto declNode = dynamic_cast<DeclNode*>(node)) {
        if (declNode->type == Variable::NUM) {
            if (!declNode->set.has_value() || declNode->set.value() == Variable::R) {
                memory.declareNum(declNode->name, Variable::R);
            }
            else if (declNode->set.value() == Variable::Z) {
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
                std::cerr << "[VM - RED FLAG] Unknown number set\n";
            }
        }
        else {
            // Pour les chaînes de caractères
            memory.declareStr(declNode->name);
        }
    }
    else if (auto assignNode = dynamic_cast<AssignNode*>(node)) {
        Value v = eval(assignNode->expr);

        if (v.isNumber())
            memory.assignNum(assignNode->name, v.asNumber());
        else if (v.isString())
            memory.assignStr(assignNode->name, v.asString());
        else
            std::cerr << "[VM - RED FLAG] Cannot assign this value type\n";
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

Value VM::eval(Node* node) {
    if (auto num = dynamic_cast<NumberNode*>(node)) {
        return Value(num->value);
    }
    if (auto str = dynamic_cast<StringNode*>(node)) {
        return Value(str->value);
    }
    if (auto bin = dynamic_cast<BinaryOpNode*>(node)) {
        Value left = eval(bin->left);
        Value right = eval(bin->right);

        if (bin->op == "+") return Value(left.asNumber() + right.asNumber());
        if (bin->op == "-") return Value(left.asNumber() - right.asNumber());
        if (bin->op == "*") return Value(left.asNumber() * right.asNumber());
        if (bin->op == "/") return Value(left.asNumber() / right.asNumber());

        std::cerr << "[VM - RED FLAG] Unknown binary operator: " << bin->op << "\n";
    }
    if (auto un = dynamic_cast<UnaryOpNode*>(node)) {
        Value v = eval(un->expr);

        if (un->op == "-")
            return Value(-v.asNumber());

        if (un->op == "+")
            return v;

        std::cerr << "[VM - RED FLAG] Unknown unary operator: " << un->op << "\n";
    }

    std::cerr << "[VM - RED FLAG] Unsupported expression in eval\n";
    return Value(); // valeur vide
}

void VM::run(const std::vector<Node*>& program) {
    for (auto node : program)
        execute(node);
}