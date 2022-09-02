#include "Entity.h"

using namespace std;

struct ParseResult {
	Entity entity;
	int index;
};

class SimpleParser {
public:
	virtual ParseResult parse(vector<Token> tokenList, int index) {
		return null;
	};
};

class ProgramParser : public SimpleParser {
public:
	ParseResult parse(vector<Token> tokenList, int index);
};

class ProcedureParser : public SimpleParser {
public:
	ParseResult parse(vector<Token> tokenList, int index);
};

class StatementParser : public SimpleParser {
public:
	ParseResult parse(vector<Token> tokenList, int index);
};

class ReadStmParser : public StatementParser {
public:
	ParseResult parse(vector<Token> tokenList, int index);
};

class PrintStmParser : public StatementParser {
public:
	ParseResult parse(vector<Token> tokenList, int index);
};

class CallStmParser : public StatementParser {
public:
	ParseResult parse(vector<Token> tokenList, int index);
};

class AssignStmParser : public StatementParser {
public:
	ParseResult parse(vector<Token> tokenList, int index);
};
