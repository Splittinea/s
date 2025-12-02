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

    // === Déclaration de variables ===
    if (t.type == TokenType::Keyword && (t.value == "num" || t.value == "str")) {
        advance(); // consomme "num" ou "str"

        Variable::Type type = (t.value == "num") ? Variable::NUM : Variable::STR;
        std::string setName = ""; // par défaut vide pour str ou réel

        // --- Si on a des parenthèses pour indiquer l'ensemble ---
        if (peek().value == "(") {
            advance(); // consomme '('
            Token setToken = advance(); // doit être Identifier (N, Z, Q, R, C)
            if (setToken.type != TokenType::Identifier) {
                throw std::runtime_error("[RED FLAG] Expected set name inside parentheses");
            }
            setName = setToken.value;

            Token closeParen = advance();
            if (closeParen.value != ")") {
                throw std::runtime_error("[RED FLAG] Expected ')' after set name");
            }
        }

        // --- Récupère le nom de la variable ---
        Token varName = advance();
        if (varName.type != TokenType::Identifier) {
            throw std::runtime_error("[RED FLAG] Expected variable name after declaration");
        }

        // Ici tu peux créer une DeclNode améliorée qui stocke le set
        return new DeclNode(varName.value, type, setName);
    }

    // === Builtins / print / input ===
    if (t.type == TokenType::Keyword && (t.value == "print" || t.value == "input")) {
        std::string kw = t.value;
        advance();

        Token open = advance();
        if (open.value != "(")
            throw std::runtime_error("[RED FLAG] Expected '(' after " + kw);

        std::vector<Node*> args;
        while (peek().value != ")") {
            args.push_back(parseExpression());
            if (peek().value == ",") advance();
        }

        Token close = advance();
        if (close.value != ")")
            throw std::runtime_error("[RED FLAG] Expected ')' after " + kw + " arguments");

        return new BuiltinNode(kw, args);
    }

    // === Assignments ===
    if (t.type == TokenType::Identifier) {
        Token varName = advance();
        Token op = advance();

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