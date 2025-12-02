#pragma once
#include <string>
#include <vector>

enum class TokenType {
	Identifier, Number, String, Keyword, Operator, Semicolon, EndOfFile
};

struct Token {
	TokenType type;
	std::string value;
	int line;
	int column;
};

class Tokenizer {
public :
	explicit Tokenizer(const std::string& source);
	std::vector<Token> tokenize();

private :
	std::string m_source;
	size_t m_pos = 0;
	int m_line = 1;
	int m_col = 1;

	char peek() const;
	char advance();
};