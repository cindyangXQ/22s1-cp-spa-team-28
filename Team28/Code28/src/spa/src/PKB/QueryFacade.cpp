#include "QueryFacade.h"

QueryFacade::QueryFacade(Storage storage) {
	this->storage = storage;
}

std::vector<Statement*> QueryFacade::getAllStatements() {
	StatementsTable* statements = (StatementsTable*)this->storage.getTable(TableName::STATEMENTS);
	
	return statements->getAll();
}

std::vector<VariableName> QueryFacade::getAllVariables() {
	VariablesTable* variables = (VariablesTable*)this->storage.getTable(TableName::VARIABLES);
	std::unordered_set names = variables->getAll();
	std::vector<VariableName> result(names.begin(), names.end());

	return result;
}

std::vector<ConstantName> QueryFacade::getAllConstants() {
	ConstantsTable* constants = (ConstantsTable*)this->storage.getTable(TableName::CONSTANTS);
	std::unordered_set names = constants->getAll();
	std::vector<ConstantName> result(names.begin(), names.end());

	return result;
}

std::vector<ProcedureName> QueryFacade::getAllProcedures() {
	ProceduresTable* constants = (ProceduresTable*)this->storage.getTable(TableName::PROCEDURES);
	std::unordered_set names = constants->getAll();
	std::vector<ProcedureName> result(names.begin(), names.end());

	return result;
}
