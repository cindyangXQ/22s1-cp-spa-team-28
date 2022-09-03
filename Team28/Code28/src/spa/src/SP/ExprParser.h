#pragma once

#include "Token.h"
#include "Parser.h"

using namespace std;

class ExprParser : public Parser {
public:
	ExprParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class TermParser :public Parser {
public:
	TermParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class FactorParser :public Parser {
public:
	FactorParser(int offset, vector<Token> tokens);
	ParseResult parse();
};
