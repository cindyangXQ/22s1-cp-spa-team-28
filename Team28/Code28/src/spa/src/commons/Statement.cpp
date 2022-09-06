#include "Statement.h"

#include <iostream>

Statement::Statement(int lineNo, StatementType type) {
	this->type = type;
	this->lineNo = lineNo;
}

Statement::Statement(int lineNo) {
    this->lineNo = lineNo;
}

StatementType Statement::getStatementType() {
	return this->type;
}

int Statement::getLineNumber(){
	return this->lineNo;
}

bool Statement::operator==(const Statement& other) const {
	return this->lineNo == other.lineNo && this->type == other.type;
}

bool Statement::isValueEqual(StatementHeader header, Statement* other) {
	switch (header) {
		case StatementHeader::LINE_NUMBER:
			return Statement::isLineNumberEqual(other);

		case StatementHeader::STATEMENT_TYPE:
			return Statement::isStatementTypeEqual(other);

		default:
			std::cerr << "Unknown StatementHeader." << std::endl;
	}
}

bool Statement::isStatementTypeEqual(Statement* other) {
	return this->type == other->type;
}

bool Statement::isLineNumberEqual(Statement* other) {
	return this->lineNo == other->lineNo;
}
