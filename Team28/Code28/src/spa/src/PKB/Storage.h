#pragma once

#include <map>
#include <string>

#include "Entity.h"
#include "Table.h"

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

	Table* getTable(TableName name);

private:
	std::map<TableName, Table*> tables;
};