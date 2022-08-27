#pragma once

#include <map>
#include <vector>

#include "Statement.h"

class StatementsTable {
public:
	StatementsTable();

	/*
	* Stores a statement into the StatementsTable.
	*/
	void store(Statement* statement);

	/*
	* Retrieves a statement from StatementsTable by index. 
	*/
	Statement retrieve(const int& index);

	/*
	* Retrieves type of a statement from StatementsTable by index.
	*/
	StatementType getStatementType(const int& index);

	/*
	* Returns the size of StatementsTable
	*/
	int getTableSize() const;

private:
	int tableSize = 0;

	map<StatementType, vector<int>> statementTypeIndexes = {
		{
			StatementType::READ, {}
		},
		{
			StatementType::PRINT, {}
		},
		{
			StatementType::ASSIGN, {}
		},
		{
			StatementType::CALL, {}
		},
		{
			StatementType::WHILE, {}
		},
		{
			StatementType::IF, {}
		},
	};

	vector<Statement*> statements;
	vector<int> indexes;
};