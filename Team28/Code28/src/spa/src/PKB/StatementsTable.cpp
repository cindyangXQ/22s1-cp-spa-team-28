#include "StatementsTable.h"
#include "Statement.h"
#include "StatementPredicateMap.h"

#include <algorithm>
#include <iostream>

StatementsTable::StatementsTable() = default;

int StatementsTable::getTableSize() const {
	return this->tableSize;
}

void StatementsTable::store(Statement* statement) {
	StatementType type = statement->getStatementType();
	int index = statement->getIndex();

	if (type == StatementType::NONE) {
		//TODO error handling
	}
	std::cout << index << std::endl;

	this->statements.push_back(statement);
	this->statementTypeIndexes[type].push_back(index);
	this->tableSize++;
}

Statement *StatementsTable::retrieve(const int& index) {
	//currently assumed that table index starts from 1
	if (index <= 0 || index > this->tableSize) {
		return nullptr;
	}

	return this->statements.at(index - 1);
}

StatementType StatementsTable::getStatementType(const int& index) {
	//currently assumed that table index starts from 1
	if (index <= 0 || index > this->tableSize) {
		//TODO error handling
		return StatementType::NONE;
	}

	Statement* statementAtIndex = this->statements.at(index - 1);
	return statementAtIndex->getStatementType();
}

StatementsTable *StatementsTable::filter(StatementPredicateMap *predicateMap) {
	if ((*predicateMap).isEmpty()) {
		return this;
	}

	StatementsTable *newTable = new StatementsTable();
	std::map<StatementHeader, Statement*> extractedMap = (*predicateMap).getPredicateMap();

	for(Statement* statement : this->statements) {
		bool isFilter = true;
		for (auto [key, val] : extractedMap) {
			if (!statement->isValueEqual(key, val)) {
				isFilter = false;
			}
		}
		if (isFilter) {
			std::cout << "Filter" << std::endl;
			newTable->store(statement);
		}
	}
	std::cout << newTable->getTableSize() << std::endl;
	std::cout << newTable->retrieve(2) << std::endl;
	std::cout << newTable->retrieve(3) << std::endl;


	return newTable;
}