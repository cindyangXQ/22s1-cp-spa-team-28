#pragma once
#include <string>

using namespace std;

class Token {
public:
	string value;
	Token();
	Token(string s);
	bool equals(string other);
	bool isName();
	bool isKeyword();
	string getValue();
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
