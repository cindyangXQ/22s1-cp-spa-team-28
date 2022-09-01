#pragma once

#include "Token.h"
#include "Node.h"

#include <vector>

class Parser {
private:
	int& offset;
	vector<Token> tokens;
public:
	inline Parser(int& offset, vector<Token> tokens);
	virtual Node parse() = 0;
};

Parser::Parser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}
