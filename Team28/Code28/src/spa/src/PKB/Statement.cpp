#include "Statement.h"

#include <iostream>

Statement::Statement(int index, StatementType type) {
	this->type = type;
	this->index = index;
}

StatementType Statement::getStatementType() {
	return this->type;
}

int Statement::getIndex(){
	return this->index;
}

bool Statement::operator==(const Statement& other) const {
	return this->index == other.index && this->type == other.type;
}

bool Statement::isValueEqual(StatementHeader header, Statement* other) {
	switch (header) {
		case StatementHeader::INDEX:
			return Statement::isIndexEqual(other);

		case StatementHeader::STATEMENT_TYPE:
			return Statement::isStatementTypeEqual(other);

		default:
			std::cerr << "Unknown StatementHeader." << std::endl;
	}
}

bool Statement::isStatementTypeEqual(Statement* other) {
	return this->type == other->type;
}

bool Statement::isIndexEqual(Statement* other) {
	return this->index == other->index;
}
