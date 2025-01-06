#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <stdexcept>

class RPN {
private:
    std::stack<int> numbers;
    bool isOperator(const char c) const;
    bool isNumber(const char c) const;
    void processOperator(const char op);
    void validateToken(const std::string& token) const;

public:
    RPN();
    ~RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);

    int calculate(const std::string& expression);
};

#endif