#include "ast/ast.h"
#include "expression_parser.h"
#include <stdexcept>

ExpressionParser::ExpressionParser(const std::vector<Token>& tokens)
    : m_tokens(tokens), m_pos(0) {}

const Token& ExpressionParser::peek() {
    if (m_pos >= m_tokens.size())
        return m_tokens.back();
    return m_tokens[m_pos];
}

const Token& ExpressionParser::advance() {
    if (m_pos < m_tokens.size())
        return m_tokens[m_pos++];
    return m_tokens.back();
}

Node* ExpressionParser::parseExpression() {
    return parseTerm();
}

Node* ExpressionParser::parseTerm() {
    Node* node = parseFactor();

    while (peek().type == TokenType::Operator &&
          (peek().value == "+" ||
           peek().value == "-")) {

        std::string op = advance().value;
        Node* right = parseFactor();

        node = new BinaryOpNode(op, node, right);
    }

    return node;
}

Node* ExpressionParser::parseFactor() {
    Node* node = parsePrimary();

    while (peek().type == TokenType::Operator &&
          (peek().value == "*" ||
           peek().value == "/" ||
           peek().value == "%")) {

        std::string op = advance().value;
        Node* right = parsePrimary();

        node = new BinaryOpNode(op, node, right);
    }

    return node;
}

Node* ExpressionParser::parsePrimary() {
    const Token& tok = peek();

    if (tok.type == TokenType::Number) {
        advance();
        return new NumberNode(std::stod(tok.value));
    }

    if (tok.type == TokenType::Identifier) {
        advance();
        return new IdentifierNode(tok.value);
    }

    if (tok.type == TokenType::Operator && tok.value == "(") {
        advance();

        Node* expr = parseExpression();

        if (!(peek().type == TokenType::Operator && peek().value == ")"))
            throw std::runtime_error("Expected ')'");

        advance();
        return expr;
    }

    throw std::runtime_error("Unexpected token in expression (primary)");
}