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
	int getOffset() { return offset; };
};

class ProgramParser : public Parser {
public:
	ProgramParser(int offset, vector<Token*> tokens) : Parser(offset, tokens) {};
	ProgramNode* parse();
};

class ProcedureParser : public Parser {
	int startline;
	vector<string> allCalls;

public:
	ProcedureParser(int offset, vector<Token*> tokens, int startline);
	ProcedureNode* parse();
	bool checkCalls(const vector<string>& procNames);
};

class StatementParser : public Parser {
protected:
	int line;

public:
	StatementParser(int offset, vector<Token*> tokens, int line);
	StatementNode* parse();
};

class ReadStmParser : public StatementParser {
public:
	ReadStmParser(int offset, vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	ReadStatementNode* parse();
};

class PrintStmParser : public StatementParser {
public:
	PrintStmParser(int offset, vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	PrintStatementNode* parse();
};

class CallStmParser : public StatementParser {
public:
	CallStmParser(int offset, vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	CallStatementNode* parse();
};

class AssignStmParser : public StatementParser {
public:
	AssignStmParser(int offset, vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	AssignStatementNode* parse();
};

class WhileStmParser : public StatementParser {
public:
	WhileStmParser(int offset, vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	WhileStatementNode* parse();
};
