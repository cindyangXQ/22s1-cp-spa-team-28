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

int StatementNode::getLineNumber() {
	return 0;
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

void ReadStatementNode::getVariablesInto(vector<string> result) {
	result.push_back(this->getVariable());
}

void ReadStatementNode::getConstantsInto(vector<string> result) {
	return;
}


// Print Statement
PrintStatementNode::PrintStatementNode(VariableNode VariableNode ) {
	this->var = VariableNode ;
}

string PrintStatementNode::getVariable() {
	return this->var.getValue();
}

void PrintStatementNode::getVariablesInto(vector<string> result) {
	result.push_back(this->getVariable());
}

void PrintStatementNode::getConstantsInto(vector<string> result) {
	return;
}

// Call Statement
CallStatementNode::CallStatementNode(VariableNode VariableNode ) {
	this->var = VariableNode ;
}

string CallStatementNode::getVariable() {
	return "";
}

void CallStatementNode::getVariablesInto(vector<string> result) {
	return;
}

void CallStatementNode::getConstantsInto(vector<string> result) {
	return;
}

// Assignment Statement
AssignStatementNode::AssignStatementNode(VariableNode VariableNode , ExpressionNode expression) {
	var = VariableNode ;
	expr = expression;
}

string AssignStatementNode::getVariable() {
	return this->var.getValue();
}

void AssignStatementNode::getVariablesInto(vector<VariableNode> result) {
	result.push_back(this->getVariable());
	this->expr.getVariablesInto(result);
}

void AssignStatementNode::getConstantsInto(vector<ConstantNode> result) {
	this->expr.getConstantsInto(result);
}

// Expression
ExpressionNode::ExpressionNode(Token* token)
{
	this->token = token;
	this->left = nullptr;
	this->right = nullptr;
}

ExpressionNode::ExpressionNode() {};

void ExpressionNode::getVariablesInto(vector<VariableNode> result) {
	if (this->token->isName()) {
		result.push_back(VariableNode(this->token->getValue()));
	}
	else if (this->token->isConstant()) {}
	else {
		this->left->getVariablesInto(result);
		this->right->getVariablesInto(result);
	}
}

void ExpressionNode::getConstantsInto(vector<ConstantNode> result) {
	if (this->token->isConstant()) {
		result.push_back(ConstantNode(this->token->getValue()));
	}
	else if (this->token->isName()) {}
	else {
		this->left->getConstantsInto(result);
		this->right->getConstantsInto(result);
	}
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
