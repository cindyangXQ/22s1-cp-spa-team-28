#include "QueryFacade.h"

QueryFacade::QueryFacade(Storage* storage) {
	this->storage = storage;
}

std::vector<Statement*> QueryFacade::getAllStatements() {
	StatementsTable* statements = (StatementsTable*)this->storage->getTable(TableName::STATEMENTS);

	return statements->getAll();
}

Statement* QueryFacade::getStatementByLineNo(const int& lineNo) {
	StatementsTable* statements = (StatementsTable*)this->storage->getTable(TableName::STATEMENTS);

	return statements->retrieve(lineNo);
}

std::vector<std::string> QueryFacade::getAllVariables() {
	VariablesTable* variables = (VariablesTable*)this->storage->getTable(TableName::VARIABLES);
	std::unordered_set names = variables->getAll();
	std::vector<std::string> result(names.begin(), names.end());

	return result;
}

Variable* QueryFacade::getVariableByName(const std::string& name) {
	VariablesTable* variables = (VariablesTable*)this->storage->getTable(TableName::VARIABLES);
	
	return variables->retrieve(name);
}

std::vector<std::string> QueryFacade::getAllConstants() {
	ConstantsTable* constants = (ConstantsTable*)this->storage->getTable(TableName::CONSTANTS);
	std::unordered_set names = constants->getAll();
	std::vector<std::string> result(names.begin(), names.end());

	return result;
}

Constant* QueryFacade::getConstantByName(const std::string& name) {
	ConstantsTable* constants = (ConstantsTable*)this->storage->getTable(TableName::CONSTANTS);

	return constants->retrieve(name);
}

std::vector<std::string> QueryFacade::getAllProcedures() {
	ProceduresTable* procedures = (ProceduresTable*)this->storage->getTable(TableName::PROCEDURES);
	std::unordered_set names = procedures->getAll();
	std::vector<std::string> result(names.begin(), names.end());

	return result;
}

Procedure* QueryFacade::getProcedureByName(const std::string& name) {
	ProceduresTable* procedures = (ProceduresTable*)this->storage->getTable(TableName::PROCEDURES);

	return procedures->retrieve(name);
}

bool QueryFacade::validate(RelationshipReference relType, Reference leftRef, Reference rightRef) {
	if (leftRef.isSynonym || rightRef.isSynonym) {
		// TODO: throw error or remove if we can assume ref passed is always correct
		return false;
	}
	switch(relType) {
		case RelationshipReference::FOLLOWS: {
			FollowsTable* follows = (FollowsTable*)this->storage->getTable(TableName::FOLLOWS);
			return follows->validate(leftRef, rightRef);
		}
		case RelationshipReference::FOLLOWS_T: {
			FollowsTTable* followsT = (FollowsTTable*)this->storage->getTable(TableName::FOLLOWS_T);
			return followsT->validate(leftRef, rightRef);
		}
		case RelationshipReference::PARENT: {
			ParentTable* parent = (ParentTable*)this->storage->getTable(TableName::PARENT);
			return parent->validate(leftRef, rightRef);
		}
		case RelationshipReference::PARENT_T: {
			ParentTTable* parentT = (ParentTTable*)this->storage->getTable(TableName::PARENT_T);
			return parentT->validate(leftRef, rightRef);
		}
		default: {
			// TODO: throw error instead of return false
			return false;
		}
	}
}

std::vector<Value> QueryFacade::solveRight(RelationshipReference relType, Reference leftRef, EntityName rightSynonym) {
	if (leftRef.isSynonym) {
		// TODO: throw error or remove if we can assume ref passed is always correct
		return std::vector<Value>();
	}
	StatementsTable* statements = (StatementsTable*)this->storage->getTable(TableName::STATEMENTS);

	switch(relType) {
		case RelationshipReference::FOLLOWS: {
			FollowsTable* follows = (FollowsTable*)this->storage->getTable(TableName::FOLLOWS);
			return follows->solveRight(leftRef, rightSynonym, statements);
		}
		case RelationshipReference::FOLLOWS_T: {
			FollowsTTable* followsT = (FollowsTTable*)this->storage->getTable(TableName::FOLLOWS_T);
			return followsT->solveRight(leftRef, rightSynonym, statements);
		}
		case RelationshipReference::PARENT: {
			ParentTable* parent = (ParentTable*)this->storage->getTable(TableName::PARENT);
			return parent->solveRight(leftRef, rightSynonym, statements);
		}
		case RelationshipReference::PARENT_T: {
			ParentTTable* parentT = (ParentTTable*)this->storage->getTable(TableName::PARENT_T);
			return parentT->solveRight(leftRef, rightSynonym, statements);	
		}
		default: {
			// TODO: throw error instead of return false
			return std::vector<Value>();
		}
	}
}

std::vector<Value> QueryFacade::solveLeft(RelationshipReference relType, Reference rightRef, EntityName leftSynonym) {
	if (rightRef.isSynonym) {
		// TODO: throw error or remove if we can assume ref passed is always correct
		return std::vector<Value>();
	}
	StatementsTable* statements = (StatementsTable*)this->storage->getTable(TableName::STATEMENTS);

	switch(relType) {
		case RelationshipReference::FOLLOWS: {
			FollowsTable* follows = (FollowsTable*)this->storage->getTable(TableName::FOLLOWS);
			return follows->solveRight(rightRef, leftSynonym, statements);
		}
		case RelationshipReference::FOLLOWS_T: {
			FollowsTTable* followsT = (FollowsTTable*)this->storage->getTable(TableName::FOLLOWS_T);
			return followsT->solveRight(rightRef, leftSynonym, statements);
		}
		case RelationshipReference::PARENT: {
			ParentTable* parent = (ParentTable*)this->storage->getTable(TableName::PARENT);
			return parent->solveRight(rightRef, leftSynonym, statements);
		}
		case RelationshipReference::PARENT_T: {
			ParentTTable* parentT = (ParentTTable*)this->storage->getTable(TableName::PARENT_T);
			return parentT->solveRight(rightRef, leftSynonym, statements);	
		}
		default: {
			// TODO: throw error instead of return false
			return std::vector<Value>();
		}
	}
}
	
std::vector<std::pair<Value, Value>> QueryFacade::solveBoth(RelationshipReference relType, EntityName leftSynonym, EntityName rightSynonym) {
	StatementsTable* statements = (StatementsTable*)this->storage->getTable(TableName::STATEMENTS);

	switch(relType) {
		case RelationshipReference::FOLLOWS: {
			FollowsTable* follows = (FollowsTable*)this->storage->getTable(TableName::FOLLOWS);
			return follows->solveBoth(leftSynonym, rightSynonym, statements);
		}
		case RelationshipReference::FOLLOWS_T: {
			FollowsTTable* followsT = (FollowsTTable*)this->storage->getTable(TableName::FOLLOWS_T);
			return followsT->solveBoth(leftSynonym, rightSynonym, statements);
		}
		case RelationshipReference::PARENT: {
			ParentTable* parent = (ParentTable*)this->storage->getTable(TableName::PARENT);
			return parent->solveBoth(leftSynonym, rightSynonym, statements);
		}
		case RelationshipReference::PARENT_T: {
			ParentTTable* parentT = (ParentTTable*)this->storage->getTable(TableName::PARENT_T);
			return parentT->solveBoth(leftSynonym, rightSynonym, statements);	
		}
		default: {
			// TODO: throw error instead of return false
			return std::vector<std::pair<Value, Value>>();
		}
	}
}
