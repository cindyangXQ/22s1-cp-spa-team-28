#include "Entity.h"
#include <vector>

using namespace std;

Program::Program(vector<Procedure> procList) {
	this->procList = procList;
}

Procedure::Procedure(vector<Statement> stmtList) {
	this->stmtList = stmtList;
}

ReadStatement::ReadStatement(Variable variable) {
	this->var = variable;
}

PrintStatement::PrintStatement(Variable variable) {
	this->var = variable;
}

CallStatement::CallStatement(Variable variable) {
	this->var = variable;
}

AssignStatement::AssignStatement(Variable variable, Expression expression) {
	this->var = variable;
	this->expr = expression;
}

Variable::Variable(string varName) {
	this.varName = varName;
}