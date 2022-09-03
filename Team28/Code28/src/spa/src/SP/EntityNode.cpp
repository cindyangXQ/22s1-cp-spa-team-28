#include "EntityNode.h"
#include <vector>

using namespace std;

EntityNode::EntityNode(){}
StatementNode::StatementNode(){}

// Program
ProgramNode::ProgramNode(vector<ProcedureNode> procList) {
	this->procList = procList;
}

vector<ProcedureNode> ProgramNode::getProcList() {
	return this->procList;
}

// Procedure
ProcedureNode::ProcedureNode(string procName, vector<StatementNode> stmtList) {
	this->procName = procName;
	this->stmtList = stmtList;
}

string ProcedureNode::getName() {
	return this->procName;
}

vector<StatementNode> ProcedureNode::getStmtList() {
	return this->stmtList;
}

// Statement - findType
bool StatementNode::isRead() {
	return false;
}
bool ReadStatementNode::isRead() {
	return true;
}
bool PrintStatementNode::isRead() {
	return false;
}
bool CallStatementNode::isRead() {
	return false;
}
bool AssignStatementNode::isRead() {
	return false;
}

bool StatementNode::isPrint() {
	return false;
}
bool ReadStatementNode::isPrint() {
	return false;
}
bool PrintStatementNode::isPrint() {
	return true;
}
bool CallStatementNode::isPrint() {
	return false;
}
bool AssignStatementNode::isPrint() {
	return false;
}

bool StatementNode::isCall() {
	return false;
}
bool ReadStatementNode::isCall() {
	return false;
}
bool PrintStatementNode::isCall() {
	return false;
}
bool CallStatementNode::isCall() {
	return true;
}
bool AssignStatementNode::isCall() {
	return false;
}

bool StatementNode::isAssign() {
	return false;
}
bool ReadStatementNode::isAssign() {
	return false;
}
bool PrintStatementNode::isAssign() {
	return false;
}
bool CallStatementNode::isAssign() {
	return false;
}
bool AssignStatementNode::isAssign() {
	return true;
}

// Read Statement
ReadStatementNode::ReadStatementNode(VariableNode VariableNode ) {
	this->var = VariableNode ;
}

// Print Statement
PrintStatementNode::PrintStatementNode(VariableNode VariableNode ) {
	this->var = VariableNode ;
}

// Call Statement
CallStatementNode::CallStatementNode(VariableNode VariableNode ) {
	this->var = VariableNode ;
}

// Assignment Statement
AssignStatementNode::AssignStatementNode(VariableNode VariableNode , ExpressionNode expression) {
	var = VariableNode ;
	expr = expression;
}

// Expression
ExpressionNode::ExpressionNode(Token* token)
{
	this->token = token;
	this->left = nullptr;
	this->right = nullptr;
}

ExpressionNode::ExpressionNode() {};


// Constant
ConstantNode ::ConstantNode (string s) {
	this->value = s;
}

bool ConstantNode::isKeyword() {
	return false;
}

bool ConstantNode ::isName() {
	return false;
}


// Variable
VariableNode ::VariableNode (string s) {
	this->value = s;
}

VariableNode ::VariableNode () {}

bool VariableNode ::isKeyword() {
	return false;
}

bool VariableNode ::isName() {
	return true;
}

bool VariableNode::isConstant() {
	return false;
}

bool ConstantNode::isConstant() {
	return true;
}
