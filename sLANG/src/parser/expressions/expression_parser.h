#pragma once
#include "ast/ast.h"
#include "lexer/tokenizer.h"

class ExpressionParser {
public:
    ExpressionParser(const std::vector<Token>& token);
    Node* parseExpression();

private:
    Node* parseTerm();
    Node* parseFactor();
    Node* parsePrimary();

    const Token& peek();
    const Token& advance();

    std::vector<Token> m_tokens;
    size_t m_pos = 0;
};