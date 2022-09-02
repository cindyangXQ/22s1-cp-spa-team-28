#include "Parser.h"
#include "Entity.h"
#include "Token.h"
#include "ExprParser.h"
#include <vector>

using namespace std;

ProgramParser::ProgramParser(int offset, vector<Token> tokens) {
	this.offset = offset;
	this->tokens = tokens;
}

ProcedureParser::ProcedureParser(int offset, vector<Token> tokens) {
	this.offset = offset;
	this->tokens = tokens;
}

StatementParser::StatementParser(int offset, vector<Token> tokens) {
	this.offset = offset;
	this->tokens = tokens;
}

ReadStmParser::ReadStmParser(int offset, vector<Token> tokens) {
	this.offset = offset;
	this->tokens = tokens;
}

PrintStmParser::PrintStmParser(int offset, vector<Token> tokens) {
	this.offset = offset;
	this->tokens = tokens;
}

CallStmParser::CallStmParser(int offset, vector<Token> tokens) {
	this.offset = offset;
	this->tokens = tokens;
}

AssignStmParser::AssignStmParser(int offset, vector<Token> tokens) {
	this.offset = offset;
	this->tokens = tokens;
}


ParseResult ProgramParser::parse() {
	int index = this.offset;
	vector<Token> tokenList = this->tokens;

	vector<Procedure> procList;
	ParseResult result;

	while (index < tokenList.size()) {
		ParseResult temp = new ProcedureParser(index, tokenList)->parse();
		procList.push_back(temp.entity);
		index = temp.index;
	}

	result = { new Program(procList), index };
	return result;
}

ParseResult ProcedureParser::parse() {
	int index = this.offset;
	vector<Token> tokenList = this->tokens;

	vector<Statement> stmtList;
	ParseResult result;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);

	if (firstToken.isKeyword() 
			&& firstToken.equals("procedure")
			&& secondToken.isName() 
			&& thirdToken.equals("{")) {
		while (!tokenList.at(index).equals("}")) {
			ParseResult temp = new StatementParser(index, tokenList)->parse();
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

	result = { new Procedure(stmtList), index + 1 };
	return result;
}

ParseResult StatementParser::parse() {
	int index = this.offset;
	vector<Token> tokenList = this->tokens;

	ParseResult result;

	string firstToken = tokenList.at(index);
	if (firstToken.equals("read")) {
		result = new ReadStmParser(index, tokenList)->parse();
	}
	else if (firstToken.equals("print")) {
		result = new PrintStmParser(index, tokenList)->parse();
	}
	else if (firstToken.equals("call")) {
		result = new CallStmParser(index, tokenList)->parse();
	}
	else {
		result = new AssignStmParser(index, tokenList)->parse();
	}
	return result;
}

ParseResult ReadStmParser::parse() {
	int index = this.offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);
	ParseResult result;

	if (firstToken.isKeyword()
			&& firstToken.equals("read")
			&& secondToken.isName()
			&& thirdToken.equals(";")) {
		result = { new ReadStatement(new Variable(secondToken.value)), index };
		return result;
	}
	else {
		throw "read statement wrong syntax";
	}
}

ParseResult PrintStmParser::parse() {
	int index = this.offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);
	ParseResult result;

	if (firstToken.isKeyword()
			&& firstToken.equals("print")
			&& secondToken.isName()
			&& thirdToken.equals(";")) {
		result = { new PrintStatement(new Variable(secondToken.value)), index };
		return result;
	}
	else {
		throw "print statement wrong syntax";
	}
}

ParseResult CallStmParser::parse() {
	int index = this.offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);
	ParseResult result;

	if (firstToken.isKeyword()
			&& firstToken.equals("call")
			&& secondToken.isName()
			&& thirdToken.equals(";")) {
		result = { new CallStatement(new Variable(secondToken.value)), index };
		return result;
	}
	else {
		throw "call statement wrong syntax";
	}
}

ParseResult AssignStmParser::parse() {
	int index = this.offset;
	vector<Token> tokenList = this->tokens;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	ParseResult temp;
	ParseResult result;

	if (firstToken.isName() && secondToken.equals("=")) {
		temp = new ExprParser(index, tokenList)->parse();
		Expression expr = temp.entity;
		index = temp.index;

		result = { new AssignStatement(new Variable(firstToken.value), expr), index };
		return result;
	}
	else {
		throw "assignment statement wrong syntax";
	}
}