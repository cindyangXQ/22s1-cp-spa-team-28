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

std::vector<VariableName> QueryFacade::getAllVariables() {
	VariablesTable* variables = (VariablesTable*)this->storage->getTable(TableName::VARIABLES);
	std::unordered_set names = variables->getAll();
	std::vector<VariableName> result(names.begin(), names.end());

	return result;
}

Variable* QueryFacade::getVariableByName(const VariableName& name) {
	VariablesTable* variables = (VariablesTable*)this->storage->getTable(TableName::VARIABLES);
	
	return variables->retrieve(name);
}

std::vector<ConstantName> QueryFacade::getAllConstants() {
	ConstantsTable* constants = (ConstantsTable*)this->storage->getTable(TableName::CONSTANTS);
	std::unordered_set names = constants->getAll();
	std::vector<ConstantName> result(names.begin(), names.end());

	return result;
}

Constant* QueryFacade::getConstantByName(const ConstantName& name) {
	ConstantsTable* constants = (ConstantsTable*)this->storage->getTable(TableName::CONSTANTS);

	return constants->retrieve(name);
}

std::vector<ProcedureName> QueryFacade::getAllProcedures() {
	ProceduresTable* procedures = (ProceduresTable*)this->storage->getTable(TableName::PROCEDURES);
	std::unordered_set names = procedures->getAll();
	std::vector<ProcedureName> result(names.begin(), names.end());

	return result;
}

Procedure* QueryFacade::getProcedureByName(const ProcedureName& name) {
	ProceduresTable* procedures = (ProceduresTable*)this->storage->getTable(TableName::PROCEDURES);

	return procedures->retrieve(name);
}
