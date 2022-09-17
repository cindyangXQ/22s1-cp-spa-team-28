#include "DesignExtractor.h"
#include "EntityNode.h"
#include "ExtractUtils.h"
#include <vector>
#include <algorithm>

DesignExtractor::DesignExtractor(ProgramNode* program, PopulateFacade* storage) {
	this->program = program;
	this->storage = storage;
}


std::vector<Procedure*> ProcedureExtractor::extract() {
	std::vector<Procedure*> result;

	std::vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(new Procedure(procList.at(i)->getName()));
	}

	return result;
}

std::vector<Statement*> StatementExtractor::extract() {
	std::vector<Statement*> result;

	std::vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		std::vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			currStmt->getStatementsInto(result);
		}
	}

	return result;
}

std::vector<Assignment*> StatementExtractor::extractAssignments() {
	std::vector<Assignment*> result;

	std::vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		std::vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			if (currStmt->isAssign()) {
				AssignStatementNode* assign = (AssignStatementNode*) currStmt;
				std::string expression = assign->getExpressionString();
				int lineNo = assign->getLineNumber();
				std::string leftVar = assign->getVariable();
				result.push_back(new Assignment(lineNo, leftVar, expression));
			}
		}
	}

	return result;
}

std::vector<Variable*> VariableExtractor::extract() {
	std::vector<std::string> preresult;
	std::vector<Variable*> result;

	std::vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		std::vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			currStmt->getVariablesInto(preresult);
		}
	}

	sort(preresult.begin(), preresult.end());
	preresult.erase(unique(preresult.begin(), preresult.end()), preresult.end());

	for (size_t i = 0; i < preresult.size(); i++) {
		result.push_back(new Variable(preresult[i]));
	}

	return result;
}

std::vector<Constant*> ConstantExtractor::extract() {
	std::vector<std::string> preresult;
	std::vector<Constant*> result;

	std::vector<ProcedureNode*> procList = program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		std::vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			currStmt->getConstantsInto(preresult);
		}
	}

	sort(preresult.begin(), preresult.end());
	preresult.erase(unique(preresult.begin(), preresult.end()), preresult.end());

	for (size_t i = 0; i < preresult.size(); i++) {
		result.push_back(new Constant(preresult[i]));
	}

	return result;
}

std::vector<Relationship<int, int>*> FollowsExtractor::extract() {
	std::vector<Relationship<int, int>*> result;

	std::vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		std::vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		ExtractUtils::follows(stmtList, result);
	}

	return result;
}

std::vector<Relationship<int, int>*> FollowsExtrT::extract() {
	std::vector<Relationship<int, int>*> result;

	std::vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		std::vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		ExtractUtils::followsT(stmtList, result);
	}

	return result;
}


void DesignExtractor::extractAll() {
	ProcedureExtractor(this->program, this->storage).populate();
	StatementExtractor(this->program, this->storage).populate();
	VariableExtractor(this->program, this->storage).populate();
	ConstantExtractor(this->program, this->storage).populate();
	FollowsExtractor(this->program, this->storage).populate();
	FollowsExtrT(this->program, this->storage).populate();
}

void ProcedureExtractor::populate() {
	std::vector<Procedure*> procedures = this->extract();
	this->storage->storeProcedures(&procedures);
}

void StatementExtractor::populate() {
	std::vector<Statement*> statements = this->extract();
	std::vector<Assignment*> assignments = this->extractAssignments();
	this->storage->storeStatements(&statements);
	this->storage->storeAssignments(&assignments);
}

void VariableExtractor::populate() {
	std::vector<Variable*> variables = this->extract();
	this->storage->storeVariables(&variables);
}

void ConstantExtractor::populate() {
	std::vector<Constant*> constants = this->extract();
	this->storage->storeConstants(&constants);
}

void FollowsExtractor::populate() {
	std::vector<Relationship<int, int>*> follows = this->extract();
	this->storage->storeFollows(&follows);
}

void FollowsExtrT::populate() {
	std::vector<Relationship<int, int>*> followsT = this->extract();
	this->storage->storeFollowsT(&followsT);
}
