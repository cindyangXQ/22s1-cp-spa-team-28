#pragma once

#include <map>
#include <string>
#include <utility>
#include <iostream>

#include "Entity.h"
#include "Table.h"
#include "TableValue.h"
#include "StatementsTable.h"
#include "NamesTable.h"
#include "NameDefinitions.h"
#include "Procedure.h"
#include "Variable.h"
#include "Constant.h"

/*
* Enumerates the different kinds of tables to instantiate.
*/
enum class TableName {
	STATEMENTS,
	PROCEDURES,
	VARIABLES,
	CONSTANTS
};

/*
* Typedef tables for brevity.
*/
typedef NamesTable<ProcedureName, Procedure> ProceduresTable;
typedef NamesTable<VariableName, Variable> VariablesTable;
typedef NamesTable<ConstantName, Constant> ConstantsTable;

/*
* Encapsulates a Storage class which is responsible for storing information to tables in PKB.
*/
class Storage {
public:
	explicit Storage() {
		StatementsTable* statements = new StatementsTable();
		ProceduresTable* procedures = new ProceduresTable();
		VariablesTable* variables = new VariablesTable();
		ConstantsTable* constants = new ConstantsTable();

		/*
		* TODO relook typecasting
		*/
		this->tables[TableName::STATEMENTS] = (Table<TableValue>*) statements;
		this->tables[TableName::PROCEDURES] = (Table<TableValue>*) procedures;
		this->tables[TableName::VARIABLES] = (Table<TableValue>*) variables;
		this->tables[TableName::CONSTANTS] = (Table<TableValue>*) constants;
	};

	Table<TableValue>* getTable(TableName name) {
		return this->tables.at(name);
	};

private:
	std::map<TableName, Table<TableValue>*> tables;
};
