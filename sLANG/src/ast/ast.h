#pragma once
#include <string>

// === CUSTOM LIBS ===
#include "core/memory.h"

// === GENERAL NODES ===
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
	IdentifierNode(const std::string& n) : name(n) {}
};

struct BuiltinNode : Node {
	std::string name;
	std::vector<Node*> args;

	BuiltinNode(const std::string& n, const std::vector<Node*>& a) : name(n), args(a) {}
};
// ================

// === VARIABLES ===
struct DeclNode : Node {
	std::string name;
	Variable::Type type;

	DeclNode(const std::string& n, Variable::Type t) : name(n), type(t) {}
};

struct AssignNode : Node {
	std::string name;
	Node* expr;

	AssignNode(const std::string& n, Node* e) : name(n), expr(e) {}
	~AssignNode() { delete expr; } // Avoid memory leaks
};
// ================

// === INSTRUCTIONS ===
struct PrintNode : Node { // print(str);
	Node* expr;			  // <= Input parameter
	PrintNode(Node* e) : expr(e) {}
};