#pragma once

#include <vector>

// === CUSTOM LIBRARIES ===
#include "ast/ast.h"
#include "core/memory.h"
#include "core/value.h"

class VM {
private:
    Memory& memory; // Memory instance reference

public:
    VM(Memory& mem) : memory(mem) {}

    void execute(Node* node);
    void run(const std::vector<Node*>& program);
    Value eval(Node* node);

};