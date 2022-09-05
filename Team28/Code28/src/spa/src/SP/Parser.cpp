#include "Parser.h"
#include "EntityNode.h"
#include "Token.h"
#include "ExprParser.h"
#include <vector>

using namespace std;

int Parser::getOffset() {
	return offset;
}

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
	cout << "[DEBUG] CallStmParser" << endl;
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

AssignStmParser::AssignStmParser(int offset, vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}


ProgramNode* ProgramParser::parse() {
	vector<Token*> tokenList = this->tokens;

	vector<ProcedureNode*> procList;
	int line = 1;

	while (offset < tokenList.size()) {
		ProcedureParser parser = ProcedureParser(offset, tokenList, line);
		ProcedureNode* temp = parser.parse();
		procList.push_back(temp);
		line = temp->getEndline() + 1;
		offset = parser.getOffset();
	}
	ProgramNode* result = new ProgramNode(procList);
	return result;
}

ProcedureNode* ProcedureParser::parse() {
	int line = this->startline;
	vector<Token*> tokenList = this->tokens;

	vector<StatementNode*> stmtList;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	Token* thirdToken = tokenList.at(offset++);

	if (firstToken->isKeyword() 
			&& firstToken->equals("procedure")
			&& secondToken->isName() 
			&& thirdToken->equals("{")) {
		while (!tokenList.at(offset)->equals("}")) {
			cout << "[DEBUG] Line number " << line << endl;
			StatementParser parser = StatementParser(offset, tokenList, line);
			StatementNode* temp = parser.parse();
			line++;
			stmtList.push_back(temp);
			offset = parser.getOffset();
			if (offset >= tokenList.size()) {
				throw "procedure wrong syntax";
			}
		}
	}
	else {
		throw "procedure wrong syntax";
	}
	offset++;
	ProcedureNode* result = new ProcedureNode(secondToken->getValue(), stmtList);
	return result;
}

StatementNode* StatementParser::parse() {
	vector<Token*> tokenList = this->tokens;
	cout << "[DEBUG] Statement First Token " << tokenList.at(offset)->value << endl;
	StatementNode* result;

	Token* firstToken = tokenList.at(offset);
	cout << "[DEBUG] First token is keyword: " << firstToken->isKeyword() << endl;

	if (firstToken->equals("read")) {
		ReadStmParser parser = ReadStmParser(offset, tokenList, line);
		result = parser.parse();
		offset = parser.getOffset();
	}
	else if (firstToken->equals("print")) {
		PrintStmParser parser = PrintStmParser(offset, tokenList, line);
		result = parser.parse();
		offset = parser.getOffset();
	}
	else if (firstToken->equals("call")) {
		cout << "[DEBUG] Call statement" << endl;
		CallStmParser parser = CallStmParser(offset, tokenList, line);
		cout << "[DEBUG] Parse result" << endl;
		result = parser.parse();
		cout << "[DEBUG] Get offset" << endl;
		offset = parser.getOffset();
	}
	else {
		cout << "[DEBUG] Assignment statement" << endl;
		AssignStmParser parser = AssignStmParser(offset, tokenList, line);
		cout << "[DEBUG] Parse result" << endl;
		result = parser.parse();
		cout << "[DEBUG] Get offset" << endl;
		offset = parser.getOffset();
	} //TODO: Need to change later


	return result;
}

ReadStatementNode* ReadStmParser::parse() {
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	Token* thirdToken = tokenList.at(offset++);

	if (firstToken->isKeyword()
			&& firstToken->equals("read")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		ReadStatementNode* result = new ReadStatementNode(VariableNode (secondToken->value), line);
		return result;
	}
	else {
		throw "read statement wrong syntax";
	}
}

PrintStatementNode* PrintStmParser::parse() {
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	Token* thirdToken = tokenList.at(offset++);

	if (firstToken->isKeyword()
			&& firstToken->equals("print")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		PrintStatementNode* result = new PrintStatementNode(VariableNode (secondToken->value), line);
		return result;
	}
	else {
		throw "print statement wrong syntax";
	}
}

CallStatementNode* CallStmParser::parse() {
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	Token* thirdToken = tokenList.at(offset++);
	cout << "[DEBUG] Parsing ... " << endl;
	cout << "[DEBUG] firstToken " << firstToken->getValue() << endl;
	cout << "[DEBUG] secondToken " << secondToken->getValue() << endl;
	cout << "[DEBUG] thirdToken " << thirdToken->getValue() << endl;
	
	if (firstToken->isKeyword()
			&& firstToken->equals("call")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		cout << "[DEBUG] Creating CallStatementNode " << endl;
		CallStatementNode* result = new CallStatementNode(VariableNode (secondToken->value), line);
		return result;
	}
	else {
		cout << "[DEBUG] Wrong syntax " << endl;
		throw "call statement wrong syntax";
	}
}

AssignStatementNode* AssignStmParser::parse() {
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	cout << "[DEBUG] Parsing ... " << endl;
	cout << "[DEBUG] firstToken " << firstToken->getValue() << endl;
	cout << "[DEBUG] secondToken " << secondToken->getValue() << endl;

	if (firstToken->isName() && secondToken->equals("=")) {
		ExprParser parser = ExprParser(offset, tokenList);
		ExpressionNode* expr = parser.parse();
		offset = parser.getOffset();

		AssignStatementNode* result = new AssignStatementNode(VariableNode (firstToken->value), expr, line);
		return result;
	}
	else {
		cout << "[DEBUG] Assignment stmt wrong syntax" << endl;
		throw "assignment statement wrong syntax";
	}
}
