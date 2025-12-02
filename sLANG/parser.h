#pragma once
#include "tokenizer.h"
#include "ast.h"
#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens)
        : m_tokens(tokens) {
    }

    // Parses the entire program and returns all the instructions
    std::vector<Node*> parseProgram();

private:
    size_t m_pos = 0;
    std::vector<Token> m_tokens;

    // Helpers
    Token peek();
    Token advance();

    Node* parseStatement();
    Node* parseExpression();
};
