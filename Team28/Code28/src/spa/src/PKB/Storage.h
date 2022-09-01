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

enum class TableName {
	STATEMENTS,
	PROCEDURES,
	VARIABLES,
	CONSTANTS
};

typedef NamesTable<ProcedureName, Procedure> ProceduresTable;
typedef NamesTable<VariableName, Variable> VariablesTable;
typedef NamesTable<ConstantName, Constant> ConstantsTable;

class Storage {
public:
	explicit Storage() {
		StatementsTable* statements = new StatementsTable();
		ProceduresTable* procedures = new ProceduresTable();
		VariablesTable* variables = new VariablesTable();
		ConstantsTable* constants = new ConstantsTable();

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