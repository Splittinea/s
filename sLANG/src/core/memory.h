#pragma once
#include <unordered_map>
#include <string>
#include <random>
#include <iostream>

class Variable {
public:
    enum Type { NUM, STR };
    enum Set { R, Z };

    Type type;
    Set set;
    double numVal;
    std::string strVal;

    Variable(double n = 0.0, Set s = R) : type(NUM), numVal(n), set(s) {}
    Variable(const std::string& s) : type(STR), strVal(s) {}
};

class Memory {
private:
    std::unordered_map<std::string, Variable> variables;

public:
    void declareNum(const std::string& name, Variable::Set set);
    void declareStr(const std::string& name);

    // Declaration with sets
    void declareInt(const std::string& name);
    // add declareComplex later
    
    void assignNum(const std::string& name, double val);
    void assignStr(const std::string& name, const std::string& val);
    Variable get(const std::string& name);
};