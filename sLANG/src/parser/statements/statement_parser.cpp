#include "statement_parser.h"
#include "core/memory.h"

StatementParser::StatementParser(const std::vector<Token>& tokens) : m_exprParser(tokens) {}

Node* StatementParser::parseStatement() {
    Token t = m_exprParser.peek();

    // === Builtins: print, input ===
    if (t.type == TokenType::Keyword && (t.value == "print" || t.value == "input")) {
        std::string kw = t.value;
        m_exprParser.advance();

        Token open = m_exprParser.advance();
        if (open.value != "(")
            throw std::runtime_error("[RED FLAG] Expected '(' after " + kw);

        std::vector<Node*> args;
        while (m_exprParser.peek().value != ")") {
            args.push_back(m_exprParser.parseExpression());
            if (m_exprParser.peek().value == ",") m_exprParser.advance();
        }

        Token close = m_exprParser.advance();
        if (close.value != ")")
            throw std::runtime_error("[RED FLAG] Expected ')' after " + kw + " arguments");

        return new BuiltinNode(kw, args);
    }

    // === Declaration: num, str ===
    if (t.type == TokenType::Keyword && (t.value == "num" || t.value == "str")) {
        std::string typeKw = t.value;
        m_exprParser.advance();

        Token next = m_exprParser.peek();
        Variable::Set setType = Variable::R;

        if (next.value == "(") {
            m_exprParser.advance();
            Token setToken = m_exprParser.advance();
            if (setToken.value == "Z") setType = Variable::Z;
            else if (setToken.value == "R") setType = Variable::R;
            else throw std::runtime_error("[RED FLAG] Unknown set type: " + setToken.value);

            Token closePar = m_exprParser.advance();
            if (closePar.value != ")")
                throw std::runtime_error("[RED FLAG] Expected ')' after set type");
        }

        Token varName = m_exprParser.advance();
        if (varName.type != TokenType::Identifier)
            throw std::runtime_error("[RED FLAG] Expected variable name after declaration");

        if (typeKw == "num") return new DeclNode(varName.value, setType);
        else return new DeclNode(varName.value, Variable::STR);
    }

    // === Assignment ===
    if (t.type == TokenType::Identifier) {
        Token varName = m_exprParser.advance();
        Token op = m_exprParser.advance();

        if (op.type == TokenType::Operator && op.value == "=") {
            Node* expr = m_exprParser.parseExpression();
            return new AssignNode(varName.value, expr);
        } else {
            throw std::runtime_error("[RED FLAG] Expected '=' in assignment");
        }
    }

    return nullptr;
}