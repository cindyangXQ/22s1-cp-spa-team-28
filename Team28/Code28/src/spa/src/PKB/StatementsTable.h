#pragma once

#include <map>
#include <vector>

#include "Statement.h"
#include "Table.h"

class StatementsTable: public Table<Statement> {
public:
	StatementsTable();

	/*
	* Stores a statement into the StatementsTable.
	*/
	void store(Statement* statement);

	/*
	* Retrieves a statement from StatementsTable by index. 
	*/
	Statement *retrieve(const int& index);

	/*
	* Retrieves type of a statement from StatementsTable by index.
	*/
	StatementType getStatementType(const int& index);

	/*
	* Returns the size of StatementsTable.
	*/
	int getTableSize() const;

private:
	int tableSize = 0;

	std::map<StatementType, std::vector<int>> statementTypeIndexes = {
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

	std::vector<Statement*> statements;
	std::vector<int> indexes;
};