#pragma once

#include "Token.h"
#include "EntityNode.h"

#include <vector>

struct ParseResult {
	EntityNode entity;
	int index;
};

class Parser {
protected:
	int offset;
	vector<Token> tokens;

public:
	inline Parser(int offset, vector<Token> tokens);
	Parser();
	virtual ParseResult parse() = 0;
};

Parser::Parser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

class ProgramParser : public Parser {
public:
	ProgramParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class ProcedureParser : public Parser {
public:
	ProcedureParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class StatementParser : public Parser {
public:
	StatementParser();
	StatementParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class ReadStmParser : public StatementParser {
public:
	ReadStmParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class PrintStmParser : public StatementParser {
public:
	PrintStmParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class CallStmParser : public StatementParser {
public:
	CallStmParser(int offset, vector<Token> tokens);
	ParseResult parse();
};

class AssignStmParser : public StatementParser {
public:
	AssignStmParser(int offset, vector<Token> tokens);
	ParseResult parse();
};
