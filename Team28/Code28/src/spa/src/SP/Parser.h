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
	vector<Token*> tokens;

public:
	Parser(int offset, vector<Token*> tokens);
	Parser();
};

class ProgramParser : public Parser {
public:
	ProgramParser(int offset, vector<Token*> tokens);
	ParseResult<ProgramNode> parse();
};

class ProcedureParser : public Parser {
protected:
	int startline;
public:
	ProcedureParser(int offset, vector<Token*> tokens, int startline);
	ParseResult<ProcedureNode> parse();
};

class StatementParser : public Parser {
protected:
	int line;
public:
	StatementParser();
	StatementParser(int offset, vector<Token*> tokens, int line);
	ParseResult<StatementNode> parse();
};

class ReadStmParser : public StatementParser {
public:
	ReadStmParser(int offset, vector<Token*> tokens, int line);
	ParseResult<ReadStatementNode> parse();
};

class PrintStmParser : public StatementParser {
public:
	PrintStmParser(int offset, vector<Token*> tokens, int line);
	ParseResult<PrintStatementNode> parse();
};

class CallStmParser : public StatementParser {
public:
	CallStmParser(int offset, vector<Token*> tokens, int line);
	ParseResult<CallStatementNode> parse();
};

class AssignStmParser : public StatementParser {
public:
	AssignStmParser(int offset, vector<Token*> tokens, int line);
	ParseResult<AssignStatementNode> parse();
};
