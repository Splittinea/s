#pragma once
#include "ast/ast.h"
#include "lexer/tokenizer.h"
#include "expressions/expression_parser.h"

class StatementParser {
public:
    StatementParser(const std::vector<Token>& tokens);
    Node* parseStatement();

private:
    ExpressionParser m_exprParser;
};