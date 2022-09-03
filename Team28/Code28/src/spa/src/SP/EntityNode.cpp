#include "EntityNode.h"
#include "Token.h"
#include <vector>

using namespace std;

ProgramNode::ProgramNode(vector<ProcedureNode> procList) {
	this->procList = procList;
}

ProcedureNode::ProcedureNode(vector<StatementNode> stmtList) {
	this->stmtList = stmtList;
}

ReadStatementNode::ReadStatementNode(Variable variable) {
	this->var = variable;
}

PrintStatementNode::PrintStatementNode(Variable variable) {
	this->var = variable;
}

CallStatementNode::CallStatementNode(Variable variable) {
	this->var = variable;
}

AssignStatementNode::AssignStatementNode(Variable variable, ExpressionNode expression) {
	this->var = variable;
	this->expr = expression;
}

ExpressionNode::ExpressionNode(Token token)
{
	this->token = token;
	this->left = nullptr;
	this->right = nullptr;
}

ExpressionNode::ExpressionNode() {};
