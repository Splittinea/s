// ==== S STANDARD LIBRARY =====
// == PACKAGE DEFINITIONS ==
#define PACKAGE_VERSION "0.0.1"
#define PACKAGE_NAME    "S Standard Library"
#define LATEST_UPDATE   "Dec 2 2025"
#define LANG            "C++"
// ==/////////////////////==

#pragma once
#include <string>
#include <vector>
#include <iostream>

// === CUSTOM LIBS ===
#include "ast/ast.h"

namespace Builtins {
    // === IO INSTRUCTIONS ===
    void print(Node* arg, Memory& memory);
    Node* input(const std::string& prompt);
    // =======================
}