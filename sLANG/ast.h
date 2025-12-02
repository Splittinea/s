#pragma once
#include <string>

struct Node {
	virtual ~Node() = default;
};

struct NumberNode : Node {
	double value;
};

struct StringNode : Node {
	std::string value;
};

struct IdentifierNode : Node {
	std::string name;
};

// === INSTRUCTIONS ===
struct PrintNode : Node { // print(str);
	Node* expr;			  // <= Input parameter
	PrintNode(Node* e) : expr(e) {}
};