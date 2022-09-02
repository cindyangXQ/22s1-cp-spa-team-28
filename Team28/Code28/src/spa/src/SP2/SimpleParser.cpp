#include "SimpleParser.h"
#include "Entity.h"
#include "Token.h"
#include <vector>

using namespace std;

ParseResult ProgramParser::parse(vector<Token> tokenList, int index) {
	vector<Procedure> procList;
	ProcedureParser procParser;
	ParserResult result;

	while (index < tokenList.size()) {
		ParseResult temp = procParser.parse(tokenList, index);
		procList.push_back(temp.entity);
		index = temp.index;
	}

	result = { new Program(procList), index };
	return result;
}

ParseResult ProcedureParser::parse(vector<Token> tokenList, int index) {
	vector<Statement> stmtList;
	StatementParser stmtParser;
	ParserResult result;

	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	Token thirdToken = tokenList.at(index++);

	if (firstToken.isKeyword() 
			&& firstToken.equals("procedure")
			&& secondToken.isName() 
			&& thirdToken.equals("{")) {
		while (!tokenList.at(index).equals("}")) {
			ParseResult temp = stmtParser.parse(tokenList, index);
			procList.push_back(temp.entity);
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

ParseResult StatementParser::parse(vector<Token> tokenList, int index) {
	StatementParser stmtParser;
	ReadStmParser readParser;
	PrintStmParser prParser;
	CallStmParser callParser;
	AssignStmParser asgParser;

	string firstToken = tokenList.at(index);
	if (firstToken.equals("read")) {
		stmtParser = readParser;
	}
	else if (firstToken.equals("print")) {
		stmtParser = prParser;
	}
	else if (firstToken.equals("call")) {
		stmtParser = callParser;
	}
	else {
		stmtParser = asgParser;
		stmt = stmtParser.parse(tokenList, index);
	}
	return stmtParser.parse(tokenList, index);
}

ParseResult ReadStmParser::parse(vector<Token> tokenList, int index) {
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

ParseResult PrintStmParser::parse(vector<Token> tokenList, int index) {
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

ParseResult CallStmParser::parse(vector<Token> tokenList, int index) {
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

ParseResult AssignStmParser::parse(vector<Token> tokenList, int index) {
	Token firstToken = tokenList.at(index++);
	Token secondToken = tokenList.at(index++);
	ExpressionParser exprParser;
	ParseResult temp;
	ParseResult result;

	if (firstToken.isName() && secondToken.equals("=")) {
		temp = exprParser.parse(tokenList, index);
		Expression expr = temp.entity;
		index = temp.index;

		result = { new AssignStatement(new Variable(firstToken.value), expr), index };
		return result;
	}
	else {
		throw "assignment statement wrong syntax";
	}
}