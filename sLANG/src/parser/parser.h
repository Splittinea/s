#pragma once
#include <vector>
#include <optional>
#include <string>
#include "ast/ast.h"
#include "lexer/tokenizer.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : m_tokens(tokens) {}

    std::vector<Node*> parseProgram();
    Node* parseExpression();
    Node* parseStatement();

    Token peek();
    Token advance();

private:
    std::vector<Token> m_tokens;
    size_t m_pos = 0;

    Node* parseAddSub();
    Node* parseMulDiv();
    Node* parseUnary();
    Node* parsePrimary();
};
