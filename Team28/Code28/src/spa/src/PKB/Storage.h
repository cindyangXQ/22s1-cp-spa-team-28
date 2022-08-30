#pragma once

#include <map>
#include <string>
#include <utility>

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

template<typename T, typename std::enable_if<std::is_base_of<TableValue, T>::value>::type* = nullptr>
class Storage {
public:
	explicit Storage() {
		StatementsTable* statements;
		NamesTable<ProcedureName, Procedure>* procedures;
		NamesTable<VariableName, Variable>* variables;
		NamesTable<ConstantName, Constant>* constants;

		this->tables[TableName::STATEMENTS] = statements;
		this->tables[TableName::PROCEDURES] = procedures;
		this->tables[TableName::VARIABLES] = variables;
		this->tables[TableName::CONSTANTS] = constants;
	};

	Table<T>* getTable(TableName name) {
		return this->tables.at(name);
	};

private:
	std::map<TableName, Table<T>*> tables;
};