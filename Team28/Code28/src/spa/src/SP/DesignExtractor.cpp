#include "DesignExtractor.h"
#include "EntityNode.h"
#include <vector>
#include <algorithm>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode* program) {
	this->program = program;
}

DesignExtractor::DesignExtractor()
{
}

StatementExtractor::StatementExtractor(ProgramNode* program)
{
	this->program = program;
}

VariableExtractor::VariableExtractor(ProgramNode* program)
{
	this->program = program;
}

// DRY needs improvement

ProcedureExtractor::ProcedureExtractor(ProgramNode* program)
{
	this->program = program;
}

vector<Procedure> ProcedureExtractor::extractor() {
	vector<Procedure> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(Procedure(procList.at(i)->getName()));
	}

	return result;
}

vector<Statement> StatementExtractor::extractor() {
	vector<Statement> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		cout << stmtList.size() << endl;
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementType type;
			//cout << stmtList.at(i) << endl;
			if (stmtList.at(j)->isRead()) {
				type = StatementType::READ;
			}
			else if (stmtList.at(j)->isPrint()) {
				type = StatementType::PRINT;
			}
			else if (stmtList.at(j)->isAssign()) {
				type = StatementType::ASSIGN;
			}
			else if (stmtList.at(j)->isCall()) {
				type = StatementType::CALL;
			}
			else {
				type = StatementType::NONE;
			}
			//cout << stmtList.at(i).getLineNumber() << endl;
			//cout << static_cast<int>(type) << endl;
			Statement statement = Statement(stmtList.at(j)->getLineNumber(), type);
			result.push_back(statement); //implement string Statement::getName(); ie. getLineNumber()
		}
	}

	return result;
}


vector<Variable> VariableExtractor::extractor() {
	vector<string> preresult;
	vector<Variable> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			currStmt->getVariablesInto(preresult);
		}
	}

	sort(preresult.begin(), preresult.end());
	preresult.erase(unique(preresult.begin(), preresult.end()), preresult.end());

	for (size_t i = 0; i < preresult.size(); i++) {
		result.push_back(Variable(preresult[i]));
	}

	return result;
}

ConstantExtractor::ConstantExtractor(ProgramNode* program)
{
	this->program = program;
}

vector<Constant> ConstantExtractor::extractor() {
	vector<string> preresult;
	vector<Constant> result;

	vector<ProcedureNode*> procList = program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			if (currStmt->isAssign()) {
				currStmt->getConstantsInto(preresult);
			}
		}
	}
	for (size_t i = 0; i < preresult.size(); i++) {
		result.push_back(Constant(preresult[i]));
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
