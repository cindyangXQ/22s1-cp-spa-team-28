#pragma once
#include <string>

class Token {
protected:
    std::string value;

public:
    Token();
    Token(std::string s);
    bool equals(std::string other);
    virtual bool equals(Token *other) { return false; }
    std::string getValue();
    virtual bool isName() { return false; };
    virtual bool isKeyword() { return false; };
    virtual bool isConstant() { return false; };
};

class Keyword : public Token {
public:
    Keyword(std::string s);
    bool isKeyword() { return true; };
    bool equals(Token *other) {
        return other->isKeyword() && other->getValue() == this->value;
    }
};

class Operator : public Token {
public:
    Operator(std::string s);
    bool equals(Token *other) { return other->getValue() == this->value; }
};

class Symbol : public Token {
public:
    Symbol(std::string s);
    bool equals(Token *other) { return other->getValue() == this->value; }
};
