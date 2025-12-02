#pragma once
#include <unordered_map>
#include <string>
#include <random>
#include <iostream>

class Variable {
public:
    enum Type { NUM, STR } type;
    double numVal;
    std::string strVal;

    Variable() : type(NUM), numVal(0) {}
    Variable(double val) : type(NUM), numVal(val) {}
    Variable(const std::string& val) : type(STR), strVal(val), numVal(0) {}
};

class Memory {
private:
    std::unordered_map<std::string, Variable> variables;

public:
    void declareNum(const std::string& name);

    void declareStr(const std::string& name);

    void assignNum(const std::string& name, double val);
    void assignStr(const std::string& name, const std::string& val);
    Variable get(const std::string& name);
};