#pragma once
#include <string>
#include <variant>
#include <iostream>

class Value {
public:
    using ValType = std::variant<double, std::string>;

    Value() : val(0.0) {}
    Value(double n) : val(n) {}
    Value(const std::string& s) : val(s) {}

    bool isNumber() const { return std::holds_alternative<double>(val); }
    bool isString() const { return std::holds_alternative<std::string>(val); }

    double asNumber() const { return std::get<double>(val); }
    const std::string& asString() const { return std::get<std::string>(val); }

private:
    ValType val;
};