#include "PKB/QueryFacade.h"

#include "catch.hpp"

TEST_CASE("getAllStatements returns all statements correctly") {
	Storage storage;
	QueryFacade facade = QueryFacade(storage);
	StatementsTable* statements = (StatementsTable*) storage.getTable(TableName::STATEMENTS);
	Statement test1 = Statement(1, StatementType::ASSIGN);
	Statement test2 = Statement(2, StatementType::ASSIGN);

	statements->store(&test1);
	statements->store(&test2);

	REQUIRE(facade.getAllStatements().size() == 2);
	REQUIRE(*facade.getAllStatements().at(0) == test1);
	REQUIRE(*facade.getAllStatements().at(1) == test2);
}
