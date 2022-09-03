#include "DesignExtractor.h"
#include "EntityNode.h"
#include <vector>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode program) {
	this->program = program;
}

DesignExtractor::DesignExtractor()
{
}

StatementExtractor::StatementExtractor(ProgramNode program)
{
	this->program = program;
}

VariableExtractor::VariableExtractor(ProgramNode program)
{
	this->program = program;
}

// DRY needs improvement

ProcedureExtractor::ProcedureExtractor(ProgramNode program)
{
	this->program = program;
}

vector<Procedure> ProcedureExtractor::extractor() {
	vector<Procedure> result;

	vector<ProcedureNode> procList = program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(Procedure(procList[i].getName()));
	}

	return result;
}

vector<Statement> StatementExtractor::extractor() {
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
			result.push_back(statement); //implement string Statement::getName(); ie. getLineNumber()
		}
	}

	return result;
}


vector<Variable> VariableExtractor::extractor() {
	vector<Variable> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode currStmt = stmtList[i];
			currStmt.getVariablesInto(result);
		}
	}

	return result;
}

ConstantExtractor::ConstantExtractor(ProgramNode program)
{
	this->program = program;
}

vector<Constant> ConstantExtractor::extractor() {
	vector<Constant> result;

	vector<ProcedureNode> procList = program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode currStmt = stmtList[i];
			if (currStmt.isAssign()) {
				currStmt.getConstantsInto(result);
			}
		}
	}

	return result;
}

/*
vector<string> DesignExtractor::extractReadStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode statement= stmtList[i];
			if (statement.isRead()) {
				result.push_back(statement.getLineNumber());
			}
		}
	}

	return result;
}

vector<string> DesignExtractor::extractPrintStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode statement = stmtList[i];
			if (statement.isPrint()) {
				result.push_back(statement.getLineNumber());
			}
		}
	}

	return result;
}

vector<string> DesignExtractor::extractCallStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode statement = stmtList[i];
			if (statement.isCall()) {
				result.push_back(statement.getLineNumber());
			}
		}
	}

	return result;
}

vector<string> DesignExtractor::extractAssignStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode statement = stmtList[i];
			if (statement.isAssign()) {
				result.push_back(statement.getLineNumber());
			}
		}
	}

	return result;
}*/

/*
vector<ModifyRel> DesignExtractor::extractModifies() {
	vector<ModifyRel> result;

	vector<ProcedureNode> procList = this->program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		ProcedureNode currProc = procList[i];
		vector<StatementNode> stmtList = currProc.getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode currStmt = stmtList[i];
			if (currStmt.isRead()) {
				string varName = currStmt.getVariable();
				result.push_back(ProcModifyRel(currProc.getName(), varName));
				result.push_back(RdStModifyRel(currStmt.getLineNumber(), varName));
			}
			else if (currStmt.isAssign()) {
				string varName = currStmt.getVariable();
				result.push_back(ProcModifyRel(currProc.getName(), varName));
				result.push_back(AsgStModifyRel(currStmt.getLineNumber(), currStmt.getVariable()));
			}
		}
	}

	return result;
}*/
