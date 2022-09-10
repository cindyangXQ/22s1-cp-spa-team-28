#pragma once

#include "Token.h"
#include "EntityNode.h"

#include <vector>

class Parser {
protected:
	int offset;
	vector<Token*> tokens;

public:
	Parser(int offset, vector<Token*> tokens);
	Parser();
	int getOffset();
};

class ProgramParser : public Parser {
public:
	ProgramParser(int offset, vector<Token*> tokens);
	ProgramNode* parse();
};

class ProcedureParser : public Parser {
protected:
	int startline;
public:
	ProcedureParser(int offset, vector<Token*> tokens, int startline);
	ProcedureNode* parse();
};

class StatementParser : public Parser {
protected:
	int line;
public:
	StatementParser();
	StatementParser(int offset, vector<Token*> tokens, int line);
	StatementNode* parse();
};

class ReadStmParser : public StatementParser {
public:
	ReadStmParser(int offset, vector<Token*> tokens, int line);
	ReadStatementNode* parse();
};

class PrintStmParser : public StatementParser {
public:
	PrintStmParser(int offset, vector<Token*> tokens, int line);
	PrintStatementNode* parse();
};

class CallStmParser : public StatementParser {
public:
	CallStmParser(int offset, vector<Token*> tokens, int line);
	CallStatementNode* parse();
};

class AssignStmParser : public StatementParser {
public:
	AssignStmParser(int offset, vector<Token*> tokens, int line);
	AssignStatementNode* parse();
};

class WhileStmParser : public StatementParser {
public:
	WhileStmParser(int offset, vector<Token*> tokens, int line);
	WhileStatementNode* parse();
};
