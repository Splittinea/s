#pragma once
#include "ast/ast.h"
#include "lexer/tokenizer.h"
#include "../statements/statement_parser.h"
#include <vector>

class ProgramParser {
public:
    ProgramParser(const std::vector<Token>& tokens);
    std::vector<Node*> parseProgram();

private:
    std::vector<Token> m_tokens;
    size_t m_pos = 0;
    StatementParser* m_stmtParser;

    const Token& peek();
    const Token& advance();
};