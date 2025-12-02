#pragma once
#include <vector>
#include <optional>
#include <cstring>

// ===== CUSTOM LIBS ====
#include "tokenizer.h"


static const TokenRule TOKEN_RULES[] = {
    // =================
    // === OPERATORS ===
    // => Logical Ops - Equality
    {"!===", TokenType::Operator}, // Not exactly equal to
    {"===", TokenType::Operator},  // Exactly equal
    {"==", TokenType::Operator},   // Equal to
    {"~=", TokenType::Operator},   // Similar to - Approx Equal to
    {"!=", TokenType::Operator},   // Not equal to

    // => Logical Ops : Comparaison
    {"<=", TokenType::Operator},   // Smaller than
    {">=", TokenType::Operator},   // Greater than

    // => Arithmetic Ops : Reduced expressions
    {"+=", TokenType::Operator},   // x += 1 => x = x + 1
    {"-=", TokenType::Operator},   // x -= 1 => x = x - 1
    {"*=", TokenType::Operator},   // x *= 1 => x = x * 1
    {"/=", TokenType::Operator},   // x /= 1 => x = x / 1
    {"%=", TokenType::Operator},   // x %= 1 => x = x % 1

    // => Arithmetic Ops : Calc
    {"+", TokenType::Operator},    // Add
    {"-", TokenType::Operator},    // Sub
    {"*", TokenType::Operator},    // Mult
    {"/", TokenType::Operator},    // Div
    {"%", TokenType::Operator},    // Modulo

    // => Assignment operator
    {"=", TokenType::Operator},
    // === ///////// ===
    // =================

    // ===============
    // === SYMBOLS ===
    // => () Expressions
    {"(", TokenType::Operator},
    {")", TokenType::Operator},

    // => {} Blocks
    {"{", TokenType::Operator},
    {"}", TokenType::Operator},

    // => Misc
    {",", TokenType::Operator},
    {":", TokenType::Operator},

    // => End of instruction
    {";", TokenType::Semicolon},
};

inline const std::vector<TokenRule> TOKEN_RULES_VEC(
    std::begin(TOKEN_RULES), std::end(TOKEN_RULES)
);

// ==============================
// Helper functions
// ==============================
std::optional<TokenRule> matchRule(const std::string& src, size_t pos);