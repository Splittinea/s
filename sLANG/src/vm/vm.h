#pragma once
#include "ast.h"
#include <variant>
#include <string>
#include <iostream>

using Value = std::variant<double, std::string>;

class VM {
public:
    VM() = default;
    void execute(Node* node);

private:
    void executePrint(PrintNode* node);
};
