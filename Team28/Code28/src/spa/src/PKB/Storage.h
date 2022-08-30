#pragma once

#include <map>
#include <string>

#include "Entity.h"
#include "Table.h"
#include "TableValue.h"

enum class TableName {
	STATEMENTS,
	PROCEDURES,
	VARIABLES,
	CONSTANTS
};

class Storage {
public:
	/*
	* Explicitly initialise all tables in PKB.
	*/
	explicit Storage();

	Table<TableValue>* getTable(TableName name);

private:
	std::map<TableName, Table<TableValue>*> tables;
};