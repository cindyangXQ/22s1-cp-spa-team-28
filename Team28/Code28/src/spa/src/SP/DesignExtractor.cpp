#include "DesignExtractor.h"
#include "EntityNode.h"
#include "ExtractUtils.h"
#include <vector>
#include <algorithm>

using namespace std;

DesignExtractor::DesignExtractor(ProgramNode* program, PopulateFacade* storage) {
	this->program = program;
	this->storage = storage;
}

vector<Procedure*> ProcedureExtractor::extract() {
	vector<Procedure*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		result.push_back(new Procedure(procList.at(i)->getName()));
	}

	return result;
}

vector<Statement*> StatementExtractor::extract() {
	vector<Statement*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			StatementNode* currStmt = stmtList.at(j);
			currStmt->getStatementsInto(result);
		}
	}

	return result;
}

vector<Variable*> VariableExtractor::extract() {
	vector<string> preresult;
	vector<Variable*> result;

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
		result.push_back(new Variable(preresult[i]));
	}

	return result;
}

vector<Constant*> ConstantExtractor::extract() {
	vector<string> preresult;
	vector<Constant*> result;

	vector<ProcedureNode*> procList = program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
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

vector<Relationship<int, int>*> FollowsExtractor::extract() {
	vector<Relationship<int, int>*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		ExtractUtils::follows(stmtList, result);
	}

	return result;
}

vector<Relationship<int, int>*> FollowsExtrT::extract() {
	vector<Relationship<int, int>*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		ExtractUtils::followsT(stmtList, result);
	}

	return result;
}

vector<Relationship<int, int>*> ParentExtractor::extract() {
	vector<Relationship<int, int>*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			ExtractUtils::parent(stmtList[j], result);
		}
	}

	return result;
}

vector<Relationship<int, int>*> ParentExtrT::extract() {
	vector<Relationship<int, int>*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			ExtractUtils::parentT(stmtList[j], result);
		}
	}

	return result;
}

vector<Relationship<int, string>*> UsesSExtractor::extract() {
	vector<Relationship<int, string>*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			stmtList[j]->getUsesInto(result);
		}
	}

	return result;
}

vector<Relationship<int, string>*> ModSExtractor::extract() {
	vector<Relationship<int, string>*> result;

	vector<ProcedureNode*> procList = this->program->getProcList();
	for (size_t i = 0; i < procList.size(); i++) {
		vector<StatementNode*> stmtList = procList.at(i)->getStmtList();
		for (size_t j = 0; j < stmtList.size(); j++) {
			stmtList[j]->getModsInto(result);
		}
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
	ParentExtractor(this->program, this->storage).populate();
	ParentExtrT(this->program, this->storage).populate();
	UsesSExtractor(this->program, this->storage).populate();
	ModSExtractor(this->program, this->storage).populate();
}

void ProcedureExtractor::populate() {
	vector<Procedure*> procedures = this->extract();
	this->storage->storeProcedures(&procedures);
}

void StatementExtractor::populate() {
	vector<Statement*> statements = this->extract();
	this->storage->storeStatements(&statements);
}

void VariableExtractor::populate() {
	vector<Variable*> variables = this->extract();
	this->storage->storeVariables(&variables);
}

void ConstantExtractor::populate() {
	vector<Constant*> constants = this->extract();
	this->storage->storeConstants(&constants);
}

void FollowsExtractor::populate() {
	vector<Relationship<int, int>*> follows = this->extract();
	this->storage->storeFollows(&follows);
}

void FollowsExtrT::populate() {
	vector<Relationship<int, int>*> followsT = this->extract();
	this->storage->storeFollowsT(&followsT);
}

void ParentExtractor::populate() {
	vector<Relationship<int, int>*> parent = this->extract();
	this->storage->storeParent(&parent);
}

void ParentExtrT::populate() {
	vector<Relationship<int, int>*> parentT = this->extract();
	this->storage->storeParentT(&parentT);
}

void UsesSExtractor::populate() {
	vector<Relationship<int, string>*> usesS = this->extract();
	this->storage->storeUsesS(&usesS);
}

void ModSExtractor::populate() {
	vector<Relationship<int, string>*> ModifiesS = this->extract();
	this->storage->storeUsesS(&ModifiesS);
}
