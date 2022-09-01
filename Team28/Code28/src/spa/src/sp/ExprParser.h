#pragma once

#include "Token.h"
#include "Node.h"
#include "Parser.h"

using namespace std;

class ExprParser: public Parser{
public:
	Node parse();
};

class TermParser :public Parser {
public:
	Node parse();
};

class FactorParser :public Parser {
public:
	Node parse();
};
