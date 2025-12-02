#pragma once

#include <vector>

// === CUSTOM LIBRARIES ===
#include "ast/ast.h"
#include "core/memory.h"

class VM {
private:
    Memory& memory; // Memory instance reference

public:
    VM(Memory& mem) : memory(mem) {}

    void execute(Node* node);
    void run(const std::vector<Node*>& program);

};