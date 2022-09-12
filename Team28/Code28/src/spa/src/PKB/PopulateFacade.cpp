#include "PopulateFacade.h"

PopulateFacade::PopulateFacade(Storage *storage) {
	this->storage = storage;
};

void PopulateFacade::storeStatements(std::vector<Statement*> *statements) {
	StatementsTable* statementsTable = (StatementsTable*) this->storage->getTable(TableName::STATEMENTS);
	
	for(Statement* statement : *statements) {
		statementsTable->store(statement);
	}	
};

void PopulateFacade::storeVariables(std::vector<Variable*> *variables) {
	VariablesTable* variableTable = (VariablesTable*) this->storage->getTable(TableName::VARIABLES);
	for(Variable* variable : *variables) {
		variableTable->store(variable);
	}
};

void PopulateFacade::storeConstants(std::vector<Constant*> *constants) {
	ConstantsTable* constantsTable = (ConstantsTable*) this->storage->getTable(TableName::CONSTANTS);
	for(Constant* constant : *constants) {
		constantsTable->store(constant);
	}
};

void PopulateFacade::storeProcedures(std::vector<Procedure*> *procedures) {
	ProceduresTable* proceduresTable = (ProceduresTable*) this->storage->getTable(TableName::PROCEDURES);
	for(Procedure* procedure : *procedures) {
		proceduresTable->store(procedure);
	}
};

void PopulateFacade::storeFollows(std::vector<Relationship<int, int>*>* follows) {
	FollowsTable* followsTable = (FollowsTable*)this->storage->getTable(TableName::FOLLOWS);
	for (Relationship<int, int>* follow : *follows) {
		followsTable->store(follow);
	}
}
