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
    return parseAddSub();
}

Node* Parser::parseAddSub() {
    Node* left = parseMulDiv();

    while (true) {
        Token t = peek();
        if (t.type == TokenType::Operator && (t.value == "+" || t.value == "-")) {
            advance();
            Node* right = parseMulDiv();
            left = new BinaryOpNode{t.value, left, right};
        } else {
            break;
        }
    }

    return left;
}

Node* Parser::parseMulDiv() {
    Node* left = parseUnary();

    while (true) {
        Token t = peek();
        if (t.type == TokenType::Operator && (t.value == "*" || t.value == "/")) {
            advance();
            Node* right = parseUnary();
            left = new BinaryOpNode{t.value, left, right};
        } else {
            break;
        }
    }

    return left;
}

Node* Parser::parseUnary() {
    Token t = peek();
    if (t.type == TokenType::Operator && (t.value == "+" || t.value == "-")) {
        advance();
        Node* expr = parseUnary();
        return new UnaryOpNode{t.value, expr};
    }
    return parsePrimary();
}

Node* Parser::parsePrimary() {
    Token t = peek();
    switch (t.type) {
        case TokenType::Number:
            advance();
            return new NumberNode{std::stod(t.value)};
        case TokenType::String:
            advance();
            return new StringNode{t.value};
        case TokenType::Identifier:
            advance();
            return new IdentifierNode{t.value};
        case TokenType::Operator:
            if (t.value == "(") {
                advance();
                Node* expr = parseExpression();
                if (peek().value != ")")
                    throw std::runtime_error("Expected ')'");
                advance();
                return expr;
            }
            break;
        default:
            throw std::runtime_error("[PARSER - RED FLAG] Unsupported expression: " + t.value);
    }
    throw std::runtime_error("[PARSER - RED FLAG] Invalid primary expression");
}


// -----------------
// Statements
// -----------------
Node* Parser::parseStatement() {
    Token t = peek();

    // === Déclaration de variables ===
    if (t.type == TokenType::Keyword && (t.value == "num" || t.value == "str")) {
        std::string typeKw = t.value;
        advance();

        std::optional<Variable::Set> setType = std::nullopt;

        Token next = peek();
        if (next.value == "(") {
            advance();
            Token setToken = advance();
            if (setToken.value == "Z") setType = Variable::Z;
            else if (setToken.value == "R") setType = Variable::R;
            else throw std::runtime_error("[RED FLAG] Unknown set type: " + setToken.value);

            Token closePar = advance();
            if (closePar.value != ")")
                throw std::runtime_error("[RED FLAG] Expected ')' after set type");
        }

        Token varName = advance();
        if (varName.type != TokenType::Identifier)
            throw std::runtime_error("[RED FLAG] Expected variable name after declaration");

        if (typeKw == "num") return new DeclNode(varName.value, Variable::NUM, setType.value_or(Variable::R));
        else return new DeclNode(varName.value, Variable::STR);
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