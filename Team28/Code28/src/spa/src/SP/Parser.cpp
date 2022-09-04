#include "Parser.h"
#include "EntityNode.h"
#include "Token.h"
#include "ExprParser.h"
#include <vector>

using namespace std;

Parser::Parser(int offset, vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

Parser::Parser(){}
StatementParser::StatementParser(){}

ProgramParser::ProgramParser(int offset, vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ProcedureParser::ProcedureParser(int offset, vector<Token*> tokens, int startline) {
	this->offset = offset;
	this->tokens = tokens;
	this->startline = startline;
}

StatementParser::StatementParser(int offset, vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

ReadStmParser::ReadStmParser(int offset, vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

PrintStmParser::PrintStmParser(int offset, vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

CallStmParser::CallStmParser(int offset, vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

AssignStmParser::AssignStmParser(int offset, vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}


ParseResult<ProgramNode> ProgramParser::parse() {
	int index = this->offset;
	vector<Token*> tokenList = this->tokens;

	vector<ProcedureNode> procList;
	int line = 1;

	while (index < tokenList.size()) {
		ParseResult<ProcedureNode> temp = ProcedureParser(index, tokenList, line).parse();
		procList.push_back(temp.entity);
		line = temp.entity.getEndline() + 1;
		index = temp.index;
	}
	ParseResult<ProgramNode> result = { ProgramNode(procList), index };
	return result;
}

ParseResult<ProcedureNode> ProcedureParser::parse() {
	int index = this->offset;
	int line = this->startline;
	vector<Token*> tokenList = this->tokens;

	vector<StatementNode> stmtList;

	Token* firstToken = tokenList.at(index++);
	Token* secondToken = tokenList.at(index++);
	Token* thirdToken = tokenList.at(index++);

	if (firstToken->isKeyword() 
			&& firstToken->equals("procedure")
			&& secondToken->isName() 
			&& thirdToken->equals("{")) {
		while (!tokenList.at(index)->equals("}")) {
			cout << line << endl;
			ParseResult<StatementNode> temp = StatementParser(index, tokenList, line).parse();
			line++;
			stmtList.push_back(temp.entity);
			index = temp.index;
			if (index >= tokenList.size()) {
				throw "procedure wrong syntax";
			}
		}
	}
	else {
		throw "procedure wrong syntax";
	}
	ParseResult<ProcedureNode> result = { ProcedureNode(secondToken->getValue(), stmtList), index + 1 };
	return result;
}

ParseResult<StatementNode> StatementParser::parse() {
	int index = this->offset;
	vector<Token*> tokenList = this->tokens;

	ParseResult<StatementNode> result;

	Token* firstToken = tokenList.at(index);
	if (firstToken->equals("read")) {
		ParseResult<ReadStatementNode> temp = ReadStmParser(index, tokenList, line).parse();
		result.index = temp.index;
		result.entity = temp.entity;
	}
	else if (firstToken->equals("print")) {
		ParseResult<PrintStatementNode> temp = PrintStmParser(index, tokenList, line).parse();
		result.index = temp.index;
		result.entity = temp.entity;
	}
	else if (firstToken->equals("call")) {
		ParseResult<CallStatementNode> temp = CallStmParser(index, tokenList, line).parse();
		result.index = temp.index;
		result.entity = temp.entity;
	}
	else {
		ParseResult<AssignStatementNode> temp = AssignStmParser(index, tokenList, line).parse();
		result.index = temp.index;
		result.entity = temp.entity;
	} //TODO: Need to change later
	return result;
}

ParseResult<ReadStatementNode> ReadStmParser::parse() {
	int index = this->offset;
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(index++);
	Token* secondToken = tokenList.at(index++);
	Token* thirdToken = tokenList.at(index++);

	if (firstToken->isKeyword()
			&& firstToken->equals("read")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		ParseResult<ReadStatementNode> result = { ReadStatementNode(VariableNode (secondToken->value), line), index };
		return result;
	}
	else {
		throw "read statement wrong syntax";
	}
}

ParseResult<PrintStatementNode> PrintStmParser::parse() {
	int index = this->offset;
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(index++);
	Token* secondToken = tokenList.at(index++);
	Token* thirdToken = tokenList.at(index++);

	if (firstToken->isKeyword()
			&& firstToken->equals("print")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		ParseResult<PrintStatementNode> result = { PrintStatementNode(VariableNode (secondToken->value), line), index };
		return result;
	}
	else {
		throw "print statement wrong syntax";
	}
}

ParseResult<CallStatementNode> CallStmParser::parse() {
	int index = this->offset;
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(index++);
	Token* secondToken = tokenList.at(index++);
	Token* thirdToken = tokenList.at(index++);

	if (firstToken->isKeyword()
			&& firstToken->equals("call")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		ParseResult<CallStatementNode> result = { CallStatementNode(VariableNode (secondToken->value), line), index };
		return result;
	}
	else {
		throw "call statement wrong syntax";
	}
}

ParseResult<AssignStatementNode> AssignStmParser::parse() {
	int index = this->offset;
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(index++);
	Token* secondToken = tokenList.at(index++);

	if (firstToken->isName() && secondToken->equals("=")) {
		ParseResult<ExpressionNode> temp = ExprParser(index, tokenList).parse();
		ExpressionNode expr = temp.entity;
		index = temp.index;

		ParseResult<AssignStatementNode> result = { AssignStatementNode(VariableNode (firstToken->value), expr, line), index };
		return result;
	}
	else {
		throw "assignment statement wrong syntax";
	}
}
