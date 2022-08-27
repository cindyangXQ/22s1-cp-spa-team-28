#include "StatementsTable.h"
#include "Statement.h"

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

	statements.push_back(statement);
	indexes.push_back(index);
	statementTypeIndexes[type].push_back(index);
	tableSize++;
}

Statement StatementsTable::retrieve(const int& index) {
	//currently assumed that table index starts from 1
	if (index <= 0 || index > tableSize) {
		return nullptr;
	}

	return statements.at(index - 1);
}

StatementType StatementsTable::getStatementType(const int& index) {
	//currently assumed that table index starts from 1
	if (index <= 0 || index > tableSize) {
		//TODO error handling
		return StatementType::NONE;
	}

	Statement statementToCheck = statements.at(index - 1);
	return statementToCheck->getStatementType();
}
