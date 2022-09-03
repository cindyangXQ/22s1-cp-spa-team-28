#include "DesignExtractor.h"
#include "EntityNode.h"
#include "Relationship.h"
#include <vector>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode program) {
	this.program = program;
}

// DRY needs improvement

vector<string> DesignExtractor::extractProcedure() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(procList[i].getName());
	}

	return result;
}

vector<string> DesignExtractor::extractStatement() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			result.push_back(stmtList[i].getLineNumber()); //implement string Statement::getName(); ie. getLineNumber()
		}
	}

	return result;
}

vector<string> DesignExtractor::extractReadStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
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

	vector<ProcedureNode> procList = this.program.getProcList();
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

	vector<ProcedureNode> procList = this.program.getProcList();
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

	vector<ProcedureNode> procList = this.program.getProcList();
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
}

vector<string> DesignExtractor::extractVariable() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode currStmt = stmtList[i];
			currStmt.getVariablesInto(result);
		}
	}

	return result;
}

vector<string> DesignExtractor::extractConstant() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
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

vector<ModifyRel> DesignExtractor::extractModifies() {
	vector<ModifyRel> result;

	vector<ProcedureNode> procList = this.program.getProcList();
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
}
