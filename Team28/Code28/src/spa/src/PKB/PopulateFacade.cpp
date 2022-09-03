#include "PopulateFacade.h"

PopulateFacade::PopulateFacade(Storage *storage) {
	this->storage = storage;
};

void PopulateFacade::storeStatement(Statement *statement) {
	StatementsTable* statements = (StatementsTable*) this->storage->getTable(TableName::STATEMENTS);
	statements->store(statement);
};

void PopulateFacade::storeVariable(Variable *variable) {
	VariablesTable* variables = (VariablesTable*) this->storage->getTable(TableName::VARIABLES);
	variables->store(variable);
};

void PopulateFacade::storeConstant(Constant *constant) {
	ConstantsTable* constants = (ConstantsTable*) this->storage->getTable(TableName::CONSTANTS);
	constants->store(constant);
};

void PopulateFacade::storeProcedure(Procedure *procedure) {
	ProceduresTable* procedures = (ProceduresTable*) this->storage->getTable(TableName::PROCEDURES);
	procedures->store(procedure);
};