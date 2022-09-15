#pragma once

#include "Token.h"
#include "EntityNode.h"

#include <vector>

class Parser {
protected:
	int offset;
	std::vector<Token*> tokens;

public:
	Parser(int offset, std::vector<Token*> tokens);
	int getOffset() { return offset; };
};

class ProgramParser : public Parser {
public:
	ProgramParser(int offset, std::vector<Token*> tokens) : Parser(offset, tokens) {};
	ProgramNode* parse();
};

class ProcedureParser : public Parser {
	int startline;
	std::vector<std::string> allCalls;

public:
	ProcedureParser(int offset, std::vector<Token*> tokens, int startline);
	ProcedureNode* parse();
	bool checkCalls(const std::vector<std::string>& procNames);
};

class StatementParser : public Parser {
protected:
	int line;

public:
	StatementParser(int offset, std::vector<Token*> tokens, int line);
	StatementNode* parse();
};

class ReadStmParser : public StatementParser {
public:
	ReadStmParser(int offset, std::vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	ReadStatementNode* parse();
};

class PrintStmParser : public StatementParser {
public:
	PrintStmParser(int offset, std::vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	PrintStatementNode* parse();
};

class CallStmParser : public StatementParser {
public:
	CallStmParser(int offset, std::vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	CallStatementNode* parse();
};

class AssignStmParser : public StatementParser {
public:
	AssignStmParser(int offset, std::vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	AssignStatementNode* parse();
};

class WhileStmParser : public StatementParser {
public:
	WhileStmParser(int offset, std::vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	WhileStatementNode* parse();
};

class IfStmParser : public StatementParser {
public:
	IfStmParser(int offset, std::vector<Token*> tokens, int line) : StatementParser(offset, tokens, line) {};
	IfStatementNode* parse();
};
