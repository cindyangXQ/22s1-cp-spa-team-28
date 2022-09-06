#pragma once

#include <map>
#include <string>
#include <utility>
#include <iostream>

#include "../commons/Entity.h"
#include "../commons/TableValue.h"
#include "../commons/NameDefinitions.h"
#include "../commons/Procedure.h"
#include "../commons/Variable.h"
#include "../commons/Constant.h"

#include "Table.h"
#include "StatementsTable.h"
#include "NamesTable.h"

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
