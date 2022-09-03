#pragma once
#include <string>

using namespace std;

class Token {
public:
	string value;
	Token();
	Token(string s);
	bool equals(string other);
	virtual bool isKeyword() {
		return false;
	};
	virtual bool isName() {
		return false;
	};
};

class ConstantToken : public Token {
public:
	ConstantToken(string s);
	bool isName();
	bool isKeyword();
};

class Name : public Token {
public:
	Name(string s);
	bool isName();
	bool isKeyword();
};

class Keyword : public Token {
public:
	Keyword(string s);
	bool isName();
	bool isKeyword();
};

class Operator : public Token {
public:
	Operator(string s);
	bool isName();
	bool isKeyword();
};

class Symbol : public Token {
public:
	Symbol(string s);
	bool isName();
	bool isKeyword();
};
