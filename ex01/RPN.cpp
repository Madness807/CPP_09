#include "RPN.hpp"
#include <sstream>
#include <cctype>

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(const RPN& other) : numbers(other.numbers) {}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
        numbers = other.numbers;
    }
    return *this;
}

bool RPN::isOperator(const char c) const {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool RPN::isNumber(const char c) const {
    return std::isdigit(c);
}

void RPN::processOperator(const char op) {
    if (numbers.size() < 2) {
        throw std::runtime_error("Error");
    }

    int b = numbers.top();
    numbers.pop();
    int a = numbers.top();
    numbers.pop();

    switch (op) {
        case '+':
            numbers.push(a + b);
            break;
        case '-':
            numbers.push(a - b);
            break;
        case '*':
            numbers.push(a * b);
            break;
        case '/':
            if (b == 0) {
                throw std::runtime_error("Error");
            }
            numbers.push(a / b);
            break;
    }
}

void RPN::validateToken(const std::string& token) const {
    if (token.length() != 1) {
        throw std::runtime_error("Error");
    }
    
    char c = token[0];
    if (!isOperator(c) && !isNumber(c)) {
        throw std::runtime_error("Error");
    }

    if (isNumber(c) && (c - '0') >= 10) {
        throw std::runtime_error("Error");
    }
}

int RPN::calculate(const std::string& expression) {
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        validateToken(token);
        char c = token[0];

        if (isNumber(c)) {
            numbers.push(c - '0');
        } else if (isOperator(c)) {
            processOperator(c);
        }
    }

    if (numbers.size() != 1) {
        throw std::runtime_error("Error");
    }

    return numbers.top();
}