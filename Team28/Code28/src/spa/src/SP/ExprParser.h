#pragma once

#include "Token.h"
#include "Parser.h"
#include "EntityNode.h"

using namespace std;

class ExprParser : public Parser {
public:
	ExprParser(int offset, vector<Token> tokens);
	ParseResult<ExpressionNode> parse();
};

class TermParser :public Parser {
public:
	TermParser(int offset, vector<Token> tokens);
	ParseResult<ExpressionNode> parse();
};

class FactorParser :public Parser {
public:
	FactorParser(int offset, vector<Token> tokens);
	ParseResult<ExpressionNode> parse();
};

Constant::Constant(string s) {
	this->value = s;
}

bool Constant::isKeyword() {
	return false;
}

bool Constant::isName() {
	return false;
}

Variable::Variable(string s) {
	this->value = s;
}

Variable::Variable() {}

bool Variable::isKeyword() {
	return false;
}

bool Variable::isName() {
	return true;
}

