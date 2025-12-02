#include "memory.h"

using namespace std;

// Functions
void Memory::declareNum(const string& name) {
    variables[name] = Variable(0.0);
}

void Memory::declareStr(const string& name) { variables[name] = Variable(""); }

void Memory::assignNum(const string& name, double val) {
    if (variables.find(name) != variables.end()){
        variables[name].type = Variable::NUM;
        variables[name].numVal = val;
    } else {
        cerr << "[RED FLAG] Undeclared variable " << name << "\n";
    }
}

void Memory::assignStr(const std::string& name, const std::string& val) {
    if (variables.find(name) != variables.end()) {
        variables[name].type = Variable::STR;
        variables[name].strVal = val;
    } else {
        std::cerr << "[RED FLAG] Undeclared variable " << name << "\n";
    }
}

Variable Memory::get(const std::string& name) {
    if (variables.find(name) != variables.end())
        return variables[name];
    throw runtime_error("[RED FLAG] Undeclared variable" + name + "\n");
}