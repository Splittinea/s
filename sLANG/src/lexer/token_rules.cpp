#include "token_rules.h"

std::optional<TokenRule> matchRule(const std::string& src, size_t pos) {
    for (const auto& rule : TOKEN_RULES) {
        size_t len = strlen(rule.text);
        if (rule.type == TokenType::Keyword) {
            if (src.compare(pos, len, rule.text) == 0 &&
                (pos + len == src.size() || !isalnum(src[pos + len]))) {
                return rule;
            }
        } else {
            if (src.compare(pos, len, rule.text) == 0) {
                return rule;
            }
        }
    }
    return std::nullopt;
}
