#include "DesignExtractor.h"
#include "EntityNode.h"
#include <vector>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode program) {
	this->program = program;
}

DesignExtractor::DesignExtractor() {}

ProcedureExtractor::ProcedureExtractor(ProgramNode program)
{
	this->program = program;
}

StatementExtractor::StatementExtractor(ProgramNode program)
{
	this->program = program;
}

VariableExtractor::VariableExtractor(ProgramNode program)
{
	this->program = program;
}

ConstantExtractor::ConstantExtractor(ProgramNode program)
{
	this->program = program;
}

// DRY needs improvement
vector<Procedure> ProcedureExtractor::extract() {
	vector<Procedure> result;

	vector<ProcedureNode> procList = program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(Procedure(procList[i].getName()));
	}

	return result;
}

vector<Statement> StatementExtractor::extract() {
	vector<Statement> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementType type;
			
			if (stmtList[i].isRead()) {
				type = StatementType::READ;
			}
			else if (stmtList[i].isPrint()) {
				type = StatementType::PRINT;
			}
			else if (stmtList[i].isAssign()) {
				type = StatementType::ASSIGN;
			}
			else if (stmtList[i].isCall()) {
				type = StatementType::CALL;
			}
			else {
				type = StatementType::NONE;
			}

			Statement statement = Statement(stmtList[i].getLineNumber(), type);
			result.push_back(statement);
		}
	}

	return result;
}


vector<Variable> VariableExtractor::extract() {
	vector<Variable> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode currStmt = stmtList[i];
			vector<Variable> currStmtVars = currStmt.getVariables();
			result.insert(result.end(), currStmtVars.begin(), currStmtVars.end());
		}
	}

	return result;
}

vector<Constant> ConstantExtractor::extract() {
	vector<Constant> result;

	vector<ProcedureNode> procList = program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode currStmt = stmtList[i];
			if (currStmt.isAssign()) {
				vector<Variable> currStmtCsts = currStmt.getConstants();
				result.insert(result.end(), currStmtCsts.begin(), currStmtCsts.end());
			}
		}
	}

	return result;
}
