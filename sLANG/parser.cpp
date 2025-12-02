#include "parser.h"
#include <stdexcept>
#include <iostream>

// =======
// Helpers
// =======
Token Parser::peek() {
    if (m_pos < m_tokens.size()) return m_tokens[m_pos];
    return Token{ TokenType::EndOfFile, "", 0, 0 };
}

Token Parser::advance() {
    if (m_pos < m_tokens.size()) return m_tokens[m_pos++];
    return Token{ TokenType::EndOfFile, "", 0, 0 };
}

// =============
// Parser Engine
// =============
Node* Parser::parseExpression() {
    Token t = peek();
    if (t.type == TokenType::Number) {
        advance();
        NumberNode* n = new NumberNode();
        n->value = std::stod(t.value);
        return n;
    }
    else if (t.type == TokenType::String) {
        advance();
        StringNode* s = new StringNode();
        s->value = t.value;
        return s;
    }
    else {
        throw std::runtime_error("[RUNTIME - RED FLAG] Unsupported Expression");
    }
}

Node* Parser::parseStatement() {
    Token t = peek();

    // ============
    // print("str")
    // ============

    if (t.type == TokenType::Keyword && t.value == "print") {
        advance();

        Token openParen = advance();
        if (openParen.value != "(")
            throw std::runtime_error("[RED FLAG] Expected '(' after print");

        Node* expr = parseExpression();

        Token closeParen = advance();
        if (closeParen.value != ")")
            throw std::runtime_error("[RED FLAG] Expected ')' after print argument");

        return new PrintNode(expr);
    }

    // Other instructions
    return nullptr;
}

std::vector<Node*> Parser::parseProgram() {
    std::vector<Node*> program;

    while (peek().type != TokenType::EndOfFile) {
        Node* stmt = parseStatement();
        if (stmt) {
            program.push_back(stmt);
        }

        // Checks for ";" char
        Token sep = peek();
        if (sep.type == TokenType::Operator && sep.value == ";") {
            advance();
        }
    }

    return program;
}
