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
