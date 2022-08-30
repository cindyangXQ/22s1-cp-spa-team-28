#include "Storage.h"
#include "StatementsTable.h"
#include "NamesTable.h"
#include "NameDefinitions.h"
#include "Procedure.h"
#include "Variable.h"
#include "Constant.h"

Storage::Storage() {
	StatementsTable statementsTable;
	NamesTable<ProcedureName, Procedure> proceduresTable;
	NamesTable<VariableName, Variable> variablesTable;
	NamesTable<ConstantName, Constant> constantsTable;

	this->tables[TableName::STATEMENTS] = &statementsTable;
	this->tables[TableName::PROCEDURES] = &proceduresTable;
	this->tables[TableName::VARIABLES] = &variablesTable;
	this->tables[TableName::CONSTANTS] = &constantsTable;
}

Table* Storage::getTable(TableName name) {
	return this->tables.at(name);
}
