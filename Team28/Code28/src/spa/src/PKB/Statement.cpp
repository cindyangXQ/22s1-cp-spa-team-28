#include "Statement.h"

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
