#include "Parser.h"
#include "EntityNode.h"
#include "Token.h"
#include "ExprParser.h"
#include <vector>

using namespace std;

// Constructors
Parser::Parser(int offset, vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ProcedureParser::ProcedureParser(int offset, vector<Token*> tokens, int startline) : Parser(offset, tokens) {
	this->startline = startline;
}

StatementParser::StatementParser(int offset, vector<Token*> tokens, int line) : Parser(offset, tokens) {
	this->line = line;
}


// Parse
ProgramNode* ProgramParser::parse() {
	vector<Token*> tokenList = this->tokens;

	vector<ProcedureNode*> procList;
	int line = 1;

	vector<string> procNames;
	vector<ProcedureParser*> allProcCalls;

	while (offset < tokenList.size()) {
		ProcedureParser* parser = new ProcedureParser(offset, tokenList, line);
		ProcedureNode* temp = parser->parse();

		// Check if procedures have unique names. 
		for (size_t i = 0; i < procNames.size(); i++) {
			if (temp->getName() == procNames[i]) {
				throw "procedure of same name is not allowed";
			}
		}
		procNames.push_back(temp->getName());
		allProcCalls.push_back(parser);

		procList.push_back(temp);

		line = temp->getEndline() + 1;
		offset = parser->getOffset();
	}

	// Check if all call statements call existing procedures.
	for (size_t i = 0; i < allProcCalls.size(); i++) {
		allProcCalls[i]->checkCalls(procNames);
	}

	return new ProgramNode(procList);
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
			StatementParser parser = StatementParser(offset, tokenList, line);
			StatementNode* temp = parser.parse();
			if (temp->isCall()) {
				if (temp->getVariable() == secondToken->getValue()) {
					throw "recursive call is not allowed";
				}
				this->allCalls.push_back(temp->getVariable());
			}
			stmtList.push_back(temp);

			line = temp->getEndLine() + 1;
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
	return new ProcedureNode(secondToken->getValue(), stmtList);
}

bool ProcedureParser::checkCalls(const vector<string>& procNames) {
	for (size_t i = 0; i < allCalls.size(); i++) {
		string temp = allCalls[i];
		bool isCallValid = false;
		for (size_t j = 0; j < procNames.size(); j++) {
			if (temp == procNames[j]) {
				isCallValid = true;
				break;
			}
		}
		if (!isCallValid) {
			throw "calling undeclared procedure is not allowed";
		}
	}
}

StatementNode* StatementParser::parse() {
	vector<Token*> tokenList = this->tokens;
	StatementNode* result;

	Token* firstToken = tokenList.at(offset);
	
	if (firstToken->isKeyword()) {
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
			CallStmParser parser = CallStmParser(offset, tokenList, line);
			result = parser.parse();
			offset = parser.getOffset();
		}
		else if (firstToken->equals("while")) {
			WhileStmParser parser = WhileStmParser(offset, tokenList, line);
			result = parser.parse();
			offset = parser.getOffset();
		}
		else if (firstToken->equals("if")) {
			IfStmParser parser = IfStmParser(offset, tokenList, line);
			result = parser.parse();
			offset = parser.getOffset();
		}
		else {
			throw "statement wrong syntax";
		}
	}
	else {
		AssignStmParser parser = AssignStmParser(offset, tokenList, line);
		result = parser.parse();
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
		return new ReadStatementNode(new VariableNode(secondToken->value), line);
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
		return new PrintStatementNode(new VariableNode(secondToken->value), line);
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
	
	if (firstToken->isKeyword()
			&& firstToken->equals("call")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		return new CallStatementNode(new VariableNode(secondToken->value), line);
	}
	else {
		throw "call statement wrong syntax";
	}
}

AssignStatementNode* AssignStmParser::parse() {
	vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);

	if (firstToken->isName() && secondToken->equals("=")) {
		ExprParser parser = ExprParser(offset, tokenList);
		ExpressionNode* expr = parser.parse();
		offset = parser.getOffset();

		return new AssignStatementNode(new VariableNode(firstToken->value), expr, line);
	}
	else {
		throw "assignment statement wrong syntax";
	}
}

WhileStatementNode* WhileStmParser::parse() {
	Token* firstToken = tokens.at(offset++);
	Token* secondToken = tokens.at(offset++);
	ExpressionNode* cond;
	int startline = this->line;

	if (secondToken->equals("(")) {
		CondParser parser = CondParser(offset, tokens);
 		cond = parser.parse();
		offset = parser.getOffset();
		if (tokens.at(offset)->equals(")")) {
			offset++;
		}
		else {
			throw "while statement wrong syntax";
		}
	}
	else {
		throw "while statement wrong syntax";
	}

	Token* curr = tokens.at(offset++);
	vector<StatementNode*> stmtList;

	if (curr->equals("{") ){
		line++;
		while (!tokens.at(offset)->equals("}")) {
			StatementParser parser = StatementParser(offset, tokens, line);
			StatementNode* temp = parser.parse();
			line++;
			stmtList.push_back(temp);
			offset = parser.getOffset();
			if (offset >= tokens.size()) {
				throw "while statement wrong syntax";
			}
		}
		offset++;
		return new WhileStatementNode(stmtList, cond, startline);
	}
	else {
		throw "while statement wrong syntax";
	}

}

IfStatementNode* IfStmParser::parse() {
	Token* firstToken = tokens.at(offset++);
	Token* secondToken = tokens.at(offset++);
	ExpressionNode* cond;
	int startline = this->line;

	if (secondToken->equals("(")) {
		CondParser parser = CondParser(offset, tokens);
		cond = parser.parse();
		offset = parser.getOffset();
		if (!tokens.at(offset++)->equals(")")||!tokens.at(offset++)->equals("then")) {
			throw "if statement wrong syntax";
		}
	}
	else {
		throw "if statement wrong syntax";
	}

	Token* curr = tokens.at(offset++);
	vector<StatementNode*> ifStmtList;
	vector<StatementNode*> elseStmtList;

	if (curr->equals("{")) {
		line++;
		while (!tokens.at(offset)->equals("}")) {
			StatementParser parser = StatementParser(offset, tokens, line);
			StatementNode* temp = parser.parse();
			line++;
			ifStmtList.push_back(temp);
			offset = parser.getOffset();
			if (offset >= tokens.size()) {
				throw "if statement wrong syntax";
			}
		}
		offset++;
	}
	else {
		throw "while statement wrong syntax";
	}

	if (!tokens.at(offset++)->equals("else") || !tokens.at(offset++)->equals("{")) {
		throw "if statement wrong syntax";
	}
	line++;
	while (!tokens.at(offset)->equals("}")) {
			StatementParser parser = StatementParser(offset, tokens, line);
			StatementNode* temp = parser.parse();
			line++;
			elseStmtList.push_back(temp);
			offset = parser.getOffset();
			if (offset >= tokens.size()) {
				throw "while statement wrong syntax";
			}
	}
	offset++;
	IfStatementNode* result = new IfStatementNode(ifStmtList, elseStmtList, cond, startline);
	return result;
}
