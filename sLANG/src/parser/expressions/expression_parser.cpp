#include "expression_parser.h"

Node* ExpressionParser::parseExpression() {
    Node* node = parseTerm();
    while (peek().type == TokenType::Operator && (peek().value == "+" || peek().value == "-")) {
        Token op = advance();
        Node* right = parseTerm();
        node = new BinaryOpNode(node, op.value, right);
    }
    return node;
}

Node* ExpressionParser::parseTerm() {
    Node* node = parseFactor();
    while (peek().type == TokenType::Operator && (peek().value == "*" || peek().value == "/")) {
        Token op = advance();
        Node* right = parseFactor();
        node = new BinaryOpNode(node, op.value, right);
    }
    return node;
}

Node* ExpressionParser::parseFactor() {
    if (peek().type == TokenType::Operator && peek().value == "-") {
        Token op = advance();
        Node* right = parseFactor();
        return new UnaryOpNode(op.value, right);
    }
    if (peek().value == "(") {
        advance();
        Node* node = parseExpression();
        if (advance().value != ")")
            throw std::runtime_error("[RED FLAG] Expected ')'");
        return node;
    }
    return parsePrimary();
}

Node* ExpressionParser::parsePrimary() {
    Token t = advance();
    switch (t.type) {
        case TokenType::Number: return new NumberNode{std::stod(t.value)};
        case TokenType::String: return new StringNode{t.value};
        case TokenType::Identifier: return new IdentifierNode{t.value};
        default: throw std::runtime_error("[RUNTIME - RED FLAG] Unsupported expression: " + t.value);
    }
}
