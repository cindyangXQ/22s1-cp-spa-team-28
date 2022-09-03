#include "EntityNode.h"
#include <vector>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode program) {
	this.program = program;
}

// DRY needs improvement

vector<string> DesignExtractor::extractProcedure() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_type i = 0; i < procList.size(); i++) {
		result.push_back(procList[i].getName());
	}

	return result;
}

vector<string> DesignExtractor::extractStatement() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_type i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_type j = 0; j < stmtList.size(); j++) {
			result.push_back(stmtList[i].getName()); //implement Statement::getName(), ie. getLineNumber()
		}
	}

	return result;
}

vector<string> DesignExtractor::extractReadStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_type i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_type j = 0; j < stmtList.size(); j++) {
			Statement statement= stmtList[i];
			if (statement.isRead()) { //implement Statement::isRead();
				result.push_back(statement.getName());
			}
		}
	}

	return result;
}

vector<string> DesignExtractor::extractPrintStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_type i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_type j = 0; j < stmtList.size(); j++) {
			Statement statement = stmtList[i];
			if (statement.isPrint()) { //implement Statement::isPrint();
				result.push_back(statement.getName());
			}
		}
	}

	return result;
}

vector<string> DesignExtractor::extractCallStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_type i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_type j = 0; j < stmtList.size(); j++) {
			Statement statement = stmtList[i];
			if (statement.isCall()) { //implement Statement::isCall();
				result.push_back(statement.getName()); 
			}
		}
	}

	return result;
}

vector<string> DesignExtractor::extractAssignStmt() {
	vector<string> result;

	vector<ProcedureNode> procList = this.program.getProcList();
	for (size_type i = 0; i < procList.size(); i++) {
		vector<StatementNode> stmtList = procList[i].getStmtList();
		for (size_type j = 0; j < stmtList.size(); j++) {
			Statement statement = stmtList[i];
			if (statement.isAssign()) { //implement Statement::isAssign();
				result.push_back(statement.getName());
			}
		}
	}

	return result;
}

vector<string> DesignExtractor::extractVariable() {
	
}

vector<string> DesignExtractor::extractConstant() {

}