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

// Procedure
ProcedureNode::ProcedureNode(string procName, vector<StatementNode*> stmtList) {
	this->procName = procName;
	this->stmtList = stmtList;
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

// Statement - findType
bool ReadStatementNode::isRead() {
	return true;
}

bool PrintStatementNode::isPrint() {
	return true;
}

bool CallStatementNode::isCall() {
	return true;
}

bool AssignStatementNode::isAssign() {
	return true;
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

// While Statement
WhileStatementNode::WhileStatementNode(vector<StatementNode*> stmtList, ExpressionNode* cond, int line)
{
	this->stmtList = stmtList;
	this->cond = cond;
	this->line = line;
}

int WhileStatementNode::getEndLine() {
	return this->stmtList.back()->getEndLine();
}

void WhileStatementNode::getVariablesInto(vector<string>& result)
{
	cond->getVariablesInto(result);
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getVariablesInto(result);
	}
}

void WhileStatementNode::getConstantsInto(vector<string>& result)
{
	cond->getConstantsInto(result);
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getConstantsInto(result);
	}
}

void WhileStatementNode::getStatementsInto(vector<Statement*>& result)
{
	result.push_back(new Statement(line, StatementType::WHILE));
	for (size_t i = 0; i < stmtList.size(); i++) {
		stmtList.at(i)->getStatementsInto(result);
	}
}

void WhileStatementNode::getFollowsInto(vector<Relationship<int, int>*>& result) {
	vector<StatementNode*> stmtList = this->getStmtList();
	int prevLine = 0;
	int currLine = 0;
	for (size_t j = 0; j < stmtList.size() - 1; j++) {
		stmtList.at(j)->getFollowsInto(&result);
		if (j == 0) {
			prevLine = stmtList.at(j)->getLineNumber();
			continue;
		}
		currLine = stmtList.at(j)->getLineNumber();
		Relationship<int, int> temp(RelationshipReference::FOLLOWS, prevLine, currLine);
		prevLine = currLine;
		result.push_back(&temp);
	}
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
