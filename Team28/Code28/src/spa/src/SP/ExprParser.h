#pragma once

#include "Parser.h"
#include "EntityNode.h"

using namespace std;

class ExprParser : public Parser {
	bool iscond;
public:
	ExprParser(int offset, vector<Token*> tokens, bool iscond);
	ExpressionNode* parse();
};

class TermParser : public Parser {
	bool iscond;
public:
	TermParser(int offset, vector<Token*> tokens, bool iscond);
	ExpressionNode* parse();
};

class FactorParser : public Parser {
	bool iscond;
public:
	FactorParser(int offset, vector<Token*> tokens, bool iscond);
	ExpressionNode* parse();
};

class CondParser : public Parser {
public:
	CondParser(int offset, vector<Token*> tokens);
	ExpressionNode* parse();
};
