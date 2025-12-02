#pragma once
#include <string>
#include <optional>

// === CUSTOM LIBS ===
#include "core/memory.h"

// === GENERAL NODES ===
struct Node {
	virtual ~Node() = default;
};

struct NumberNode : Node {
	double value;

	NumberNode() : value(0.0) {}
    NumberNode(double v) : value(v) {}
};

struct StringNode : Node {
	std::string value;

	StringNode() : value("") {}
    StringNode(const std::string& v) : value(v) {}
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

struct BinaryOpNode : Node {
    Node* left;
    std::string op;
    Node* right;

    BinaryOpNode(const std::string& op, Node* left, Node* right)
    : op(op), left(left), right(right) {}
};

struct UnaryOpNode : Node {
    std::string op;
    Node* expr;
    UnaryOpNode(const std::string& o, Node* e) : op(o), expr(e) {}
    ~UnaryOpNode() { delete expr; }
};
// ================

// === VARIABLES ===
struct DeclNode : Node {
    std::string name;
    Variable::Type type;
    std::optional<Variable::Set> set;

    DeclNode(const std::string& n, Variable::Type t, Variable::Set s)
        : name(n), type(t), set(s) {}

    DeclNode(const std::string& n, Variable::Type t)
        : name(n), type(t), set(std::nullopt) {}
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