#include "EntityNode.h"
#include <vector>

using namespace std;

EntityNode::EntityNode(){}
StatementNode::StatementNode() { this->line = 0; }

// Program
ProgramNode::ProgramNode(vector<ProcedureNode*> procList) {
	this->procList = procList;
}

ProgramNode::ProgramNode()
{
}

vector<ProcedureNode*> ProgramNode::getProcList() {
	return this->procList;
}

bool ProgramNode::equals(ProgramNode other) {
	vector<ProcedureNode*> procedures = this->getProcList();
	vector<ProcedureNode*> others = other.getProcList();
	if (procedures.size() != others.size()) {
		return false;
	}
	for (int i = 0; i < procedures.size(); i++) {
		if (!procedures[i]->equals(*others[i])) {
			return false;
		}
	}
	return true;
}

// Procedure
ProcedureNode::ProcedureNode(string procName, vector<StatementNode*> stmtList) {
	this->procName = procName;
	this->stmtList = stmtList;
}

bool ProcedureNode::equals(ProcedureNode other) {
	if (this->getName() != other.getName()) {
		return false;
	}
	vector<StatementNode*> statements = this->getStmtList();
	vector<StatementNode*> others = other.getStmtList();
	if (statements.size() != others.size()) {
		return false;
	}
	for (int i = 0; i < statements.size(); i++) {
		if (!statements[i]->equals(*others[i])) {
			return false;
		}
	}
	return true;
}

string ProcedureNode::getName() {
	return this->procName;
}

vector<StatementNode*> ProcedureNode::getStmtList() {
	return this->stmtList;
}

int ProcedureNode::getEndline()
{
	return stmtList.back()->getLineNumber();
}

int StatementNode::getLineNumber() {
	return this->line;
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

// Statement - equals
bool ReadStatementNode::equals(StatementNode other) {
	return other.isRead() && this->getVariable() == other.getVariable();
}

bool PrintStatementNode::equals(StatementNode other) {
	return other.isPrint() && this->getVariable() == other.getVariable();
}

bool CallStatementNode::equals(StatementNode other) {
	return other.isCall() && this->getVariable() == other.getVariable();
}

bool AssignStatementNode::equals(StatementNode other) {
	// Expressions not checked
	std::cout << "isAssign: " << other.isAssign() << endl;
	std::cout << "this->getVariable: " << this->getVariable() << endl;
	std::cout << "other.getVariable: " << other.getVariable() << endl;
	return other.isAssign() && this->getVariable() == other.getVariable();
}

// Read Statement
ReadStatementNode::ReadStatementNode(VariableNode VariableNode, int line) {
	this->var = VariableNode ;
	this->line = line;
}

string ReadStatementNode::getVariable() {
	return this->var.getValue();
}

void ReadStatementNode::getVariablesInto(vector<string>& result) {
	result.push_back(this->getVariable());
}

void ReadStatementNode::getConstantsInto(vector<string>& result) {
	return;
}

// Print Statement
PrintStatementNode::PrintStatementNode(VariableNode VariableNode, int line ) {
	this->var = VariableNode ;
	this->line = line;
}

string PrintStatementNode::getVariable() {
	return this->var.getValue();
}

void PrintStatementNode::getVariablesInto(vector<string>& result) {
	result.push_back(this->getVariable());
}

void PrintStatementNode::getConstantsInto(vector<string>& result) {
	return;
}

// Call Statement
CallStatementNode::CallStatementNode(VariableNode VariableNode, int line ) {
	this->var = VariableNode ;
	this->line = line;
}

string CallStatementNode::getVariable() {
	return "";
}

void CallStatementNode::getVariablesInto(vector<string>& result) {
	return;
}

void CallStatementNode::getConstantsInto(vector<string>& result) {
	return;
}

// Assignment Statement
AssignStatementNode::AssignStatementNode(VariableNode VariableNode , ExpressionNode* expression, int line) {
	var = VariableNode ;
	expr = expression;
	this->line = line;
}

string AssignStatementNode::getVariable() {
	return this->var.getValue();
}

void AssignStatementNode::getVariablesInto(vector<string>& result) {
	result.push_back(this->getVariable());
	this->expr->getVariablesInto(result);
}

void AssignStatementNode::getConstantsInto(vector<string>& result) {
	this->expr->getConstantsInto(result);
}

// Expression
ExpressionNode::ExpressionNode(Token* token)
{
	this->token = token;
	this->left = NULL;
	this->right = NULL;
}

ExpressionNode::ExpressionNode() {};

void ExpressionNode::getVariablesInto(vector<string>& result) {
	if (this->token->isName()) {
		result.push_back(this->token->getValue());
	}
	else if (this->token->isConstant()) {}
	else {
		this->left->getVariablesInto(result);
		this->right->getVariablesInto(result);
	}
}

void ExpressionNode::getConstantsInto(vector<string>& result) {
	if (this->token->isConstant()) {
		result.push_back(this->token->getValue());
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
