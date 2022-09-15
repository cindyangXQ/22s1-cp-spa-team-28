#include "Parser.h"
#include "EntityNode.h"
#include "Token.h"
#include "ExprParser.h"
#include <vector>

std::string REL_LIST[] = { "!", ">", "<", "==", "!=", ">=", "<=","&&", "||" };

int Parser::getOffset() {
	return offset;
}

// Constructors
Parser::Parser(int offset, std::vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

Parser::Parser(){}
StatementParser::StatementParser(){}

ProgramParser::ProgramParser(int offset, std::vector<Token*> tokens) {
	this->offset = offset;
	this->tokens = tokens;
}

ProcedureParser::ProcedureParser(int offset, std::vector<Token*> tokens, int startline) {
	this->offset = offset;
	this->tokens = tokens;
	this->startline = startline;
}

StatementParser::StatementParser(int offset, std::vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

ReadStmParser::ReadStmParser(int offset, std::vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

PrintStmParser::PrintStmParser(int offset, std::vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

CallStmParser::CallStmParser(int offset, std::vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

AssignStmParser::AssignStmParser(int offset, std::vector<Token*> tokens, int line) {
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}

WhileStmParser::WhileStmParser(int offset, std::vector<Token*> tokens, int line)
{
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}


// Parse
ProgramNode* ProgramParser::parse() {
	std::vector<Token*> tokenList = this->tokens;

	std::vector<ProcedureNode*> procList;
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
	std::vector<Token*> tokenList = this->tokens;

	std::vector<StatementNode*> stmtList;

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
			line = temp->getEndLine() + 1;
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
	std::vector<Token*> tokenList = this->tokens;
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
	std::vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	Token* thirdToken = tokenList.at(offset++);

	if (firstToken->isKeyword()
			&& firstToken->equals("read")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		ReadStatementNode* result = new ReadStatementNode(VariableNode(secondToken->value), line);
		return result;
	}
	else {
		throw "read statement wrong syntax";
	}
}

PrintStatementNode* PrintStmParser::parse() {
	std::vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	Token* thirdToken = tokenList.at(offset++);

	if (firstToken->isKeyword()
			&& firstToken->equals("print")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		PrintStatementNode* result = new PrintStatementNode(VariableNode(secondToken->value), line);
		return result;
	}
	else {
		throw "print statement wrong syntax";
	}
}

CallStatementNode* CallStmParser::parse() {
	std::vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);
	Token* thirdToken = tokenList.at(offset++);
	
	if (firstToken->isKeyword()
			&& firstToken->equals("call")
			&& secondToken->isName()
			&& thirdToken->equals(";")) {
		CallStatementNode* result = new CallStatementNode(VariableNode(secondToken->value), line);
		return result;
	}
	else {
		throw "call statement wrong syntax";
	}
}

AssignStatementNode* AssignStmParser::parse() {
	std::vector<Token*> tokenList = this->tokens;

	Token* firstToken = tokenList.at(offset++);
	Token* secondToken = tokenList.at(offset++);

	if (firstToken->isName() && secondToken->equals("=")) {
		ExprParser parser = ExprParser(offset, tokenList, false);
		ExpressionNode* expr = parser.parse();
		offset = parser.getOffset();

		AssignStatementNode* result = new AssignStatementNode(VariableNode(firstToken->value), expr, line);
		return result;
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
		if (std::find(std::begin(REL_LIST), std::end(REL_LIST), cond->getToken()->value) == std::end(REL_LIST)) {
			throw "invalid cond expression";
		}
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
	std::vector<StatementNode*> stmtList;

	if (curr->equals("{") ){
		line++;
		while (!tokens.at(offset)->equals("}")) {
			StatementParser parser = StatementParser(offset, tokens, line);
			StatementNode* temp = parser.parse();
			line = temp->getEndLine() + 1;
			stmtList.push_back(temp);
			offset = parser.getOffset();
			if (offset >= tokens.size()) {
				throw "while statement wrong syntax";
			}
		}
		offset++;
		WhileStatementNode* result = new WhileStatementNode(stmtList, cond, startline);
		return result;
	}
	else {
		throw "while statement wrong syntax";
	}

}

IfStmParser::IfStmParser(int offset, std::vector<Token*> tokens, int line)
{
	this->offset = offset;
	this->tokens = tokens;
	this->line = line;
}


IfStatementNode* IfStmParser::parse() {
	Token* firstToken = tokens.at(offset++);
	Token* secondToken = tokens.at(offset++);
	ExpressionNode* cond;
	int startline = this->line;

	if (secondToken->equals("(")) {
		CondParser parser = CondParser(offset, tokens);
		cond = parser.parse();
		if (std::find(std::begin(REL_LIST), std::end(REL_LIST), cond->getToken()->value) == std::end(REL_LIST)) {
			throw "invalid cond expression";
		}
		offset = parser.getOffset();
		if (!tokens.at(offset++)->equals(")")||!tokens.at(offset++)->equals("then")) {
			throw "if statement wrong syntax";
		}
	}
	else {
		throw "if statement wrong syntax";
	}

	Token* curr = tokens.at(offset++);
	std::vector<StatementNode*> ifStmtList;
	std::vector<StatementNode*> elseStmtList;

	if (curr->equals("{")) {
		line++;
		while (!tokens.at(offset)->equals("}")) {
			StatementParser parser = StatementParser(offset, tokens, line);
			StatementNode* temp = parser.parse();
			line = temp->getEndLine() + 1;
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
	while (!tokens.at(offset)->equals("}")) {
			StatementParser parser = StatementParser(offset, tokens, line);
			StatementNode* temp = parser.parse();
			line = temp->getEndLine() + 1;
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
