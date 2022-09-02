#pragma once

#ifndef Token_HEADER
#define Token_HEADER
#include <string>

using namespace std;

class Token {
public:
	string value;
	Token();
	Token(string s);
};

class Constant : public Token {
public:
	Constant(string s);
};

class Name : public Token {
public:
	Name(string s);
};

class Keyword : public Token {
public:
	Keyword(string s);
};

class Operator : public Token {
public:
	Operator(string s);
};

class Symbol : public Token {
public:
	Symbol(string s);
};
#endif
