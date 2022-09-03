#include "EntityNode.h"
#include <vector>

using namespace std;

EntityNode::EntityNode(){}
StatementNode::StatementNode(){}

// Program
ProgramNode::ProgramNode(vector<ProcedureNode> procList) {
	this->procList = procList;
}

ProgramNode::ProgramNode()
{
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

int StatementNode::getLineNumber() {
	return line;
}

// Statement - findType
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

string ReadStatementNode::getVariable() {
	return this->var.getValue();
}

vector<Variable*> ReadStatementNode::getVariables() {
	vector<Variable*> result;
	result.push_back(&Variable(this->getVariable()));
	return result;
}

vector<Constant*> ReadStatementNode::getConstants() {
	return vector<Constant*>();
}

// Print Statement
PrintStatementNode::PrintStatementNode(VariableNode VariableNode ) {
	this->var = VariableNode ;
}

string PrintStatementNode::getVariable() {
	return this->var.getValue();
}

vector<Variable*> PrintStatementNode::getVariables() {
	vector<Variable*> result;
	result.push_back(&Variable(this->getVariable()));
	return result;
}

vector<Constant*> PrintStatementNode::getConstants() {
	return vector<Constant*>();
}

// Call Statement
CallStatementNode::CallStatementNode(VariableNode VariableNode ) {
	this->var = VariableNode ;
}

string CallStatementNode::getVariable() {
	return "";
}

vector<Variable*> CallStatementNode::getVariables() {
	return vector<Variable*>();
}

vector<Constant*> CallStatementNode::getConstants() {
	return vector<Constant*>();
}

// Assignment Statement
AssignStatementNode::AssignStatementNode(VariableNode VariableNode, ExpressionNode expression) {
	var = VariableNode;
	expr = expression;
}

string AssignStatementNode::getVariable() {
	return this->var.getValue();
}

vector<Variable*> AssignStatementNode::getVariables() {
	vector<Variable*> result;
	result.push_back(&Variable(this->getVariable()));
	vector<Variable*> inExpr = this->expr.getVariables();
	result.insert(result.end(), inExpr.begin(), inExpr.end());
	return result;
}

vector<Constant*> AssignStatementNode::getConstants() {
	return this->expr.getConstants();
}

// Expression
ExpressionNode::ExpressionNode(Token* token)
{
	this->token = token;
	this->left = nullptr;
	this->right = nullptr;
}

ExpressionNode::ExpressionNode() {};

vector<Variable*> ExpressionNode::getVariables() {
	vector<Variable*> result;

	if (this->token->isName()) {
		result.push_back(&Variable(this->token->getValue()));
	}
	else if (this->token->isConstant()) {}
	else {
		vector<Variable*> inLeft = this->left->getVariables();
		vector<Variable*> inRight = this->right->getVariables();
		result.insert(result.end(), inLeft.begin(), inLeft.end());
		result.insert(result.end(), inRight.begin(), inRight.end());
	}

	return result;
}

vector<Constant*> ExpressionNode::getConstants() {
	vector<Constant*> result;
	
	if (this->token->isConstant()) {
		result.push_back(&Constant(this->token->getValue()));
	}
	else if (this->token->isName()) {}
	else {
		vector<Constant*> inLeft = this->left->getConstants();
		vector<Constant*> inRight = this->right->getConstants();
		result.insert(result.end(), inLeft.begin(), inLeft.end());
		result.insert(result.end(), inRight.begin(), inRight.end());
	}
	
	return result;
}


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

bool ConstantNode::isConstant() {
	return true;
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
