#include "QueryFacade.h"

QueryFacade::QueryFacade(Storage storage) {
	this->storage = storage;
}

std::vector<Statement*> QueryFacade::getAllStatements() {
	StatementsTable* statements = (StatementsTable*)this->storage.getTable(TableName::STATEMENTS);
	return statements->getAll();
}
