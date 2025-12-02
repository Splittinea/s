#pragma once
#include "ast/ast.h"
#include "lexer/tokenizer.h"

class ExpressionParser {
public:
    ExpressionParser(const std::vector<Token>& token);
    Node* parseExpression();

    const Token& peek();
    const Token& advance();
private:
    Node* parseTerm();
    Node* parseFactor();
    Node* parsePrimary();

    std::vector<Token> m_tokens;
    size_t m_pos = 0;
};