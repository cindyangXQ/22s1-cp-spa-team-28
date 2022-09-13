#include "EntityNode.h"
#include "ExtractUtils.h"
#include <vector>

using namespace std;

EntityNode::EntityNode(){}
StatementNode::StatementNode() { this->line = 0; }

// Program
ProgramNode::ProgramNode(vector<ProcedureNode*> procList) {
	this->procList = procList;
}

ProgramNode::ProgramNode() {}

vector<ProcedureNode*> ProgramNode::getProcList() {
	return this->procList;
}

bool ProgramNode::equals(ProgramNode* other) {
	vector<ProcedureNode*> procedures = this->getProcList();
	vector<ProcedureNode*> others = other->getProcList();
	return ExtractUtils::compareProcList(procedures, others);
}

// Procedure
ProcedureNode::ProcedureNode(string procName, vector<StatementNode*> stmtList) {
	this->procName = procName;
	this->stmtList = stmtList;
}

bool ProcedureNode::equals(ProcedureNode* other) {
	if (this->getName() != other->getName()) {
		return false;
	}
	vector<StatementNode*> statements = this->getStmtList();
	vector<StatementNode*> others = other->getStmtList();
	return ExtractUtils::compareStmtList(statements, others);
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


// Statement - equals
bool ReadStatementNode::equals(StatementNode* other) {
	return other->isRead() && this->getVariable() == other->getVariable();
}

bool PrintStatementNode::equals(StatementNode* other) {
	return other->isPrint() && this->getVariable() == other->getVariable();
}

bool CallStatementNode::equals(StatementNode* other) {
	return other->isCall() && this->getVariable() == other->getVariable();
}

bool AssignStatementNode::equals(StatementNode* other) {
	// Expressions not checked
	return other->isAssign() && this->getVariable() == other->getVariable();
}

bool WhileStatementNode::equals(StatementNode* other) {
	// Conditional and Statements not checked
	return other->isWhile();
}

// Read Statement
ReadStatementNode::ReadStatementNode(const VariableNode& VariableNode, int line) {
	this->var = VariableNode ;
	this->line = line;
}

string ReadStatementNode::getVariable() {
	return this->var.getValue();
}

void ReadStatementNode::getVariablesInto(vector<string>& result) {
	result.push_back(this->getVariable());
}

void ReadStatementNode::getStatementsInto(vector<Statement*>& result) { 
	result.push_back(new Statement(line, StatementType::READ)); 
}

// Print Statement
PrintStatementNode::PrintStatementNode(const VariableNode& VariableNode, int line ) {
	this->var = VariableNode ;
	this->line = line;
}

string PrintStatementNode::getVariable() {
	return this->var.getValue();
}

void PrintStatementNode::getVariablesInto(vector<string>& result) {
	result.push_back(this->getVariable());
}

void PrintStatementNode::getStatementsInto(vector<Statement*>& result) {
	result.push_back(new Statement(line, StatementType::PRINT)); 
}

// Call Statement
CallStatementNode::CallStatementNode(const VariableNode& VariableNode, int line ) {
	this->var = VariableNode ;
	this->line = line;
}

void CallStatementNode::getStatementsInto(vector<Statement*>& result) { 
	result.push_back(new Statement(line, StatementType::CALL)); 
}

// Assignment Statement
AssignStatementNode::AssignStatementNode(const VariableNode& VariableNode , ExpressionNode* expression, int line) {
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

void AssignStatementNode::getStatementsInto(vector<Statement*>& result) { 
	result.push_back(new Statement(line, StatementType::ASSIGN)); 
}

// While Statement
WhileStatementNode::WhileStatementNode(const vector<StatementNode*>& stmtList, ExpressionNode* cond, int line)
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
	ExtractUtils::follows(this->getStmtList(), result);
}

void WhileStatementNode::getFollowsTInto(vector<Relationship<int, int>*>& result) {
	ExtractUtils::followsT(this->getStmtList(), result);
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
