#include "program_parser.h"
#include <stdexcept>

ProgramParser::ProgramParser(const std::vector<Token>& tokens) : m_tokens(tokens) {
    m_stmtParser = new StatementParser(tokens);
}

const Token& ProgramParser::peek() {
    if (m_pos < m_tokens.size()) return m_tokens[m_pos];
    static Token eofToken{TokenType::EndOfFile, "", 0, 0};
    return eofToken;
}

const Token& ProgramParser::advance() {
    if (m_pos < m_tokens.size()) return m_tokens[m_pos++];
    static Token eofToken{TokenType::EndOfFile, "", 0, 0};
    return eofToken;
}

std::vector<Node*> ProgramParser::parseProgram() {
    std::vector<Node*> program;

    while (peek().type != TokenType::EndOfFile) {
        Node* stmt = m_stmtParser->parseStatement();
        if (stmt) program.push_back(stmt);

        Token sep = peek();
        if (sep.type == TokenType::Semicolon) advance();
        else throw std::runtime_error("[RED FLAG] ';' expected after statement");
    }

    return program;
}
