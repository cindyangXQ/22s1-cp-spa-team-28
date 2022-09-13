#include "../commons/Statement.h"
#include "StatementsTable.h"
#include "StatementPredicateMap.h"

#include <algorithm>

StatementsTable::StatementsTable() = default;

int StatementsTable::getTableSize() const {
	return this->tableSize;
}

void StatementsTable::store(Statement* statement) {
	StatementType type = statement->getStatementType();
	int index = statement->getLineNumber();

	if (type == StatementType::NONE) {
		//TODO error handling
	}

	this->statements.push_back(statement);
	this->statementTypeIndexes[type].push_back(index);
	this->tableSize++;
}

Statement *StatementsTable::retrieve(const int& lineNum) {
	// currently assumes that lineNum is unique
	for(Statement* statement : this->statements) {
		if (statement->getLineNumber() == lineNum) {
			return statement;
		}
	}

	return nullptr;
}

StatementType StatementsTable::getStatementType(const int& lineNum) {
	// currently assumes that lineNum is unique
	for(Statement* statement : this->statements) {
		if (statement->getLineNumber() == lineNum) {
			return statement->getStatementType();
		}
	}

	return StatementType::NONE;

}

StatementsTable *StatementsTable::filter(StatementPredicateMap *predicateMap) {
	if ((*predicateMap).isEmpty()) {
		return this;
	}

	StatementsTable *newTable = new StatementsTable();
	std::map<StatementHeader, Statement*> extractedMap = (*predicateMap).getPredicateMap();

	for(Statement* statement : this->statements) {
		bool isFilter = true;
		for (auto const&[key, val] : extractedMap) {
			if (!statement->isValueEqual(key, val)) {
				isFilter = false;
			}
		}
		if (isFilter) {
			newTable->store(statement);
		}
	}

	return newTable;
}

std::vector<Statement*> StatementsTable::getAll() {
	return this->statements;
}

std::vector<int> StatementsTable::getAllLineNumbers() {
	std::vector<Statement*> statements = this->statements;
	std::vector<int> lineNumbers;
	std::transform(statements.begin(), statements.end(), 
		std::back_inserter(lineNumbers), Statement::getLineNumberFromStatement);
	return lineNumbers;
}

std::vector<int> StatementsTable::getStatementsByType(StatementType type) {
	return this->statementTypeIndexes[type];
}
