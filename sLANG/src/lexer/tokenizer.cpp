#include "tokenizer.h"
#include <cctype>

Tokenizer::Tokenizer(const std::string& source) : m_source(source) {}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;
    while (m_pos < m_source.size()) {
        char c = m_source[m_pos];

        if (std::isspace(c)) {
            m_pos++;
            continue;
        }

        if (c == '(' || c == ')') {
            tokens.push_back(Token{ TokenType::Operator, std::string(1, c), m_line, m_col++ });
            m_pos++;
            continue;
        }

        if (std::isalpha(c)) {
            std::string word;
            while (m_pos < m_source.size() && std::isalnum(m_source[m_pos])) {
                word += m_source[m_pos++];
            }

            TokenType type = TokenType::Identifier;
            if (word == "print" || word == "num" || word == "str") {
                type = TokenType::Keyword;
            }

            tokens.push_back(Token{ type, word, m_line, m_col });
            continue;
        }

        if (std::isdigit(c) || c == '.') {
            std::string number;
            while (m_pos < m_source.size() &&
                (std::isdigit(m_source[m_pos]) || m_source[m_pos] == '.')) {
                number += m_source[m_pos++];
            }
            tokens.push_back(Token{ TokenType::Number, number, m_line, m_col });
            continue;
        }

        if (c == '"') {
            m_pos++;
            std::string str;
            while (m_pos < m_source.size() && m_source[m_pos] != '"') {
                str += m_source[m_pos++];
            }
            m_pos++;
            tokens.push_back(Token{ TokenType::String, str, m_line, m_col });
            continue;
        }

        if (c == ';') {
            tokens.push_back(Token{ TokenType::Semicolon, ";", m_line, m_col++ });
            m_pos++;
            continue;
        }

        if (c == '=' || c == '+' || c == '-' || c == '*' || c == '/') {
            tokens.push_back(Token{ TokenType::Operator, std::string(1, c), m_line, m_col++ });
            m_pos++;
            continue;
        }

        if (c == '=' || c == '!' || c == '<' || c == '>') {
            std::string op;
            op += c;
            m_pos++;

            // check for 2-char operator
            if (m_pos < m_source.size() && m_source[m_pos] == '=') {
                op += '=';
                m_pos++;
            }

            tokens.push_back(Token{ TokenType::Operator, op, m_line, m_col++ });
            continue;
        }
        
        m_pos++;
    }

    tokens.push_back(Token{ TokenType::EndOfFile, "", m_line, m_col });
    return tokens;
}