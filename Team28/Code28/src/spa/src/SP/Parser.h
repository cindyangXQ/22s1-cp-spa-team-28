#pragma once

#include "Token.h"
#include "EntityNode.h"

#include <vector>

template <typename T>
struct ParseResult {
	T entity;
	int index;
};

class Parser {
protected:
	int offset;
	vector<Token> tokens;

public:
	Parser(int offset, vector<Token> tokens);
	Parser();
};

class ProgramParser : public Parser {
public:
	ProgramParser(int offset, vector<Token> tokens);
	ParseResult<ProgramNode> parse();
};

class ProcedureParser : public Parser {
public:
	ProcedureParser(int offset, vector<Token> tokens);
	ParseResult<ProcedureNode> parse();
};

class StatementParser : public Parser {
public:
	StatementParser();
	StatementParser(int offset, vector<Token> tokens);
	ParseResult<StatementNode> parse();
};

class ReadStmParser : public StatementParser {
public:
	ReadStmParser(int offset, vector<Token> tokens);
	ParseResult<ReadStatementNode> parse();
};

class PrintStmParser : public StatementParser {
public:
	PrintStmParser(int offset, vector<Token> tokens);
	ParseResult<PrintStatementNode> parse();
};

class CallStmParser : public StatementParser {
public:
	CallStmParser(int offset, vector<Token> tokens);
	ParseResult<CallStatementNode> parse();
};

class AssignStmParser : public StatementParser {
public:
	AssignStmParser(int offset, vector<Token> tokens);
	ParseResult<AssignStatementNode> parse();
};
