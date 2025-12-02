#include <cctype>
#include <vector>
#include <string>

// === CUSTOM LIBS ===
#include "tokenizer.h"
#include "token_rules.h"

Tokenizer::Tokenizer(const std::string& source) : m_source(source) {}

char Tokenizer::peek() const {
    return m_pos < m_source.size() ? m_source[m_pos] : '\0';
}

char Tokenizer::advance() {
    if (m_pos < m_source.size()) {
        char c = m_source[m_pos++];
        m_col++;
        return c;
    }
    return '\0';
}

// === HELPERS ===
bool isKeyword(const std::string& word) {
    static const std::vector<std::string> keywords = {"print", "num", "str"};
    for (auto& kw : keywords) if (word == kw) return true;
    return false;
}
// ===============

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    while (m_pos < m_source.size()) {
        char c = peek();

        // Skips whitespaces
        if (std::isspace(c)) {
            if (c == '\n') {
                m_line++;
                m_col = 1;
            }
            advance();
            continue;
        }

        // --- Match operators / symbols / semicolons ---
        if (auto ruleOpt = matchRule(m_source, m_pos)) {
            const auto& rule = *ruleOpt;
            size_t len = strlen(rule.text);

            tokens.push_back(Token{rule.type, rule.text, m_line, m_col});

            for (size_t i = 0; i < len; i++) advance();
            continue;
        }

        // --- Identifiers & Keywords ---
        if (std::isalpha(c) || c == '_') {
            std::string word;
            while (m_pos < m_source.size() &&
                   (std::isalnum(m_source[m_pos]) || m_source[m_pos] == '_')) {
                word += advance();
            }

            TokenType type = isKeyword(word) ? TokenType::Keyword : TokenType::Identifier;
            tokens.push_back(Token{type, word, m_line, m_col});
            continue;
        }

        // --- Numbers ---
        if (std::isdigit(c) || c == '.') {
            std::string number;
            while (m_pos < m_source.size() &&
                   (std::isdigit(peek()) || peek() == '.')) {
                number += advance();
            }
            tokens.push_back(Token{TokenType::Number, number, m_line, m_col});
            continue;
        }

        // --- Strings ---
        if (c == '"') {
            advance(); // skip "
            std::string str;
            while (m_pos < m_source.size() && peek() != '"') {
                str += advance();
            }
            if (peek() == '"') advance(); // skip closing "
            tokens.push_back(Token{TokenType::String, str, m_line, m_col});
            continue;
        }

        // --- Unknown character ---
        // Ignore or trigger an error
        advance();
    }

    tokens.push_back(Token{TokenType::EndOfFile, "", m_line, m_col});
    return tokens;
}