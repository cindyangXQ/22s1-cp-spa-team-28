#include "Parser.h"
#include "EntityNode.h"
#include "Token.h"
#include "ExprParser.h"
#include <vector>

using namespace std;

Parser::Parser(){}
StatementParser::StatementParser(){}

ProgramParser::ProgramParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ProcedureParser::ProcedureParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

StatementParser::StatementParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ReadStmParser::ReadStmParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

PrintStmParser::PrintStmParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

CallStmParser::CallStmParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

AssignStmParser::AssignStmParser(int offset, vector<Token> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}


ParseResult ProgramParser::parse() {
	int index = this->offset;
	vector<Token> tokenList = this->tokens;

	vector<ProcedureNode> procList;
	ParseResult result;

	while (index < tokenList.size()) {
		ParseResult temp = ProcedureParser(index, tokenList).parse();
		procList.push_back(dynamic_cast<StatementNode*>(temp.entity));
		index = temp.index;
	}

	result = { ProgramNode(procList), index };
	return result;
}

ParseResult ProcedureParser::parse() {
	int index = this->offset;
	vector<Token> tokenList = this->tokens;

	vector<StatementNode> stmtList;
	ParseResult result;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);

	if (firstToken.isKeyword() 
			&& firstToken.equals("procedure")
			&& secondToken.isName() 
			&& thirdToken.equals("{")) {
		while (!tokenList.at(index).equals("}")) {
			ParseResult temp = StatementParser(index, tokenList).parse();
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

	result = { ProcedureNode(stmtList), index + 1 };
	return result;
}

ParseResult StatementParser::parse() {
	int index = this->offset;
	vector<Token> tokenList = this->tokens;

	ParseResult result;

	Token firstToken = tokenList.at(index);
	if (firstToken.equals("read")) {
		result = ReadStmParser(index, tokenList).parse();
	}
	else if (firstToken.equals("print")) {
		result = PrintStmParser(index, tokenList).parse();
	}
	else if (firstToken.equals("call")) {
		result = CallStmParser(index, tokenList).parse();
	}
	else {
		result = AssignStmParser(index, tokenList).parse();
	}
	return result;
}

ParseResult ReadStmParser::parse() {
	int index = this->offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);
	ParseResult result;

	if (firstToken.isKeyword()
			&& firstToken.equals("read")
			&& secondToken.isName()
			&& thirdToken.equals(";")) {
		result = { ReadStatementNode(Variable(secondToken.value)), index };
		return result;
	}
	else {
		throw "read statement wrong syntax";
	}
}

ParseResult PrintStmParser::parse() {
	int index = this->offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);
	ParseResult result;

	if (firstToken.isKeyword()
			&& firstToken.equals("print")
			&& secondToken.isName()
			&& thirdToken.equals(";")) {
		result = { PrintStatementNode(Variable(secondToken.value)), index };
		return result;
	}
	else {
		throw "print statement wrong syntax";
	}
}

ParseResult CallStmParser::parse() {
	int index = this->offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);
	ParseResult result;

	if (firstToken.isKeyword()
			&& firstToken.equals("call")
			&& secondToken.isName()
			&& thirdToken.equals(";")) {
		result = { CallStatementNode(Variable(secondToken.value)), index };
		return result;
	}
	else {
		throw "call statement wrong syntax";
	}
}

ParseResult AssignStmParser::parse() {
	int index = this->offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	ParseResult temp;
	ParseResult result;

	if (firstToken.isName() && secondToken.equals("=")) {
		temp = ExprParser(index, tokenList).parse();
		ExpressionNode expr = temp.entity;
		index = temp.index;

		result = { AssignStatementNode(Variable(firstToken.value), expr), index };
		return result;
	}
	else {
		throw "assignment statement wrong syntax";
	}
}
