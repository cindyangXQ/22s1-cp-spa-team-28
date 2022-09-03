#pragma once

#include "Parser.h"
#include "EntityNode.h"

using namespace std;

class ExprParser : public Parser {
public:
	ExprParser(int offset, vector<Token*> tokens);
	ParseResult<ExpressionNode> parse();
};

class TermParser :public Parser {
public:
	TermParser(int offset, vector<Token*> tokens);
	ParseResult<ExpressionNode> parse();
};

class FactorParser :public Parser {
public:
	FactorParser(int offset, vector<Token*> tokens);
	ParseResult<ExpressionNode> parse();
};

