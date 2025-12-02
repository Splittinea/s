#include <stdexcept>
#include <iostream>

// === CUSTOM LIBS ===
#include "parser.h"
#include "core/memory.h"
#include "lexer/token_rules.h"

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

    switch (t.type) {
        case TokenType::Number: {
            advance();
            NumberNode* n = new NumberNode();
            n->value = std::stod(t.value);
            return n;
        }
        case TokenType::String: {
            advance();
            StringNode* s = new StringNode();
            s->value = t.value;
            return s;
        }
        case TokenType::Identifier: {
            advance();
            return new IdentifierNode{ t.value };
        }
        default:
            throw std::runtime_error("[RUNTIME - RED FLAG] Unsupported expression: " + t.value);
    }
}

// -----------------
// Statements
// -----------------
Node* Parser::parseStatement() {
    Token t = peek();

    // --- PRINT ---
    if (t.type == TokenType::Keyword && t.value == "print") {
        advance();

        Token openParen = advance();

        if (!(openParen.type == TokenType::Operator && openParen.value == "("))
            throw std::runtime_error("[RED FLAG] Expected '(' after print");

        Node* expr = parseExpression();

        Token closeParen = advance();
        if (!(closeParen.type == TokenType::Operator && closeParen.value == ")"))
            throw std::runtime_error("[RED FLAG] Expected ')' after print argument");

        return new PrintNode(expr);
    }

    // --- DECLARATION ---
    if (t.type == TokenType::Keyword && (t.value == "num" || t.value == "str")) {
        advance(); // consume keyword

        Token varName = advance();
        if (varName.type != TokenType::Identifier)
            throw std::runtime_error("[RED FLAG] Expected variable name after declaration");

        Variable::Type type = (t.value == "num") ? Variable::NUM : Variable::STR;
        return new DeclNode(varName.value, type);
    }

    // --- ASSIGNMENT ---
    if (t.type == TokenType::Identifier) {
        Token varName = advance();
        Token op = advance();

        // On vérifie que l'opérateur fait partie de TOKEN_RULES
        if (op.type == TokenType::Operator && op.value == "=") {
            Node* expr = parseExpression();
            return new AssignNode(varName.value, expr);
        } else {
            throw std::runtime_error("[RED FLAG] Expected '=' in assignment");
        }
    }

    return nullptr;
}

// -----------------
// Program
// -----------------
std::vector<Node*> Parser::parseProgram() {
    std::vector<Node*> program;

    while (peek().type != TokenType::EndOfFile) {
        Node* stmt = parseStatement();
        if (stmt) program.push_back(stmt);

        // --- Check for semicolon ---
        Token sep = peek();
        if (sep.type == TokenType::Semicolon) {
            advance();
        } else {
            throw std::runtime_error("[RED FLAG] ';' expected after statement");
        }
    }

    return program;
}