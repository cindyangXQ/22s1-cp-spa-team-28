#include "commons/NameDefinitions.h"
#include "commons/Constant.h"
#include "commons/Variable.h"
#include "commons/Procedure.h"
#include "PKB/NamesTable.h"
#include "PKB/EntityPredicateMap.h"

#include "catch.hpp"

TEST_CASE("NamesTable is initialised correctly") {
	NamesTable<ConstantName, Constant> table;

	REQUIRE(table.getTableSize() == 0);
}

TEST_CASE("NamesTable<ConstantName, Constant> can store and retrieve correctly") {
	NamesTable<ConstantName, Constant> table;
	Constant test = Constant("Test");
	
	table.store(&test);

	// test is stored and retrieved correctly
	REQUIRE(*table.retrieve(test.getName()) == test);
	// invalid index returns a nullptr for retrieve()
	REQUIRE(table.retrieve((ConstantName)"not test") == nullptr);
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 1);
}

TEST_CASE("NamesTable<VariableName, Variable> can store and retrieve correctly") {
	NamesTable<VariableName, Variable> table;
	Variable test = Variable("Test");

	table.store(&test);

	// test is stored and retrieved correctly
	REQUIRE(*table.retrieve(test.getName()) == test);
	// invalid index returns a nullptr for retrieve()
	REQUIRE(table.retrieve((VariableName)"not test") == nullptr);
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 1);
}

TEST_CASE("NamesTable<ProcedureName, Procedure> can store and retrieve correctly") {
	NamesTable<ProcedureName, Procedure> table;
	Procedure test = Procedure("Test");

	table.store(&test);

	// test is stored and retrieved correctly
	REQUIRE(*table.retrieve(test.getName()) == test);
	// invalid index returns a nullptr for retrieve()
	REQUIRE(table.retrieve((ProcedureName)"not test") == nullptr);
	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 1);
}

TEST_CASE("NamesTable<ConstantName, Constant> can store 10 items correctly") {
	ConstantsTable table;
	Constant a = Constant("a");
	table.store(&a);
	Constant b = Constant("b");
	table.store(&b);
	Constant c = Constant("c");
	table.store(&c);

	REQUIRE(*table.retrieve(a.getName()) == a);
	REQUIRE(*table.retrieve("b") == b);
	REQUIRE(table.getTableSize() == 3);
}

TEST_CASE("Successfully filters ConstantsTable using ConstantName") {
	ConstantsTable table;
	Constant testName = Constant("b"); 
	std::map<EntityHeader, Constant*> m = {{EntityHeader::NAME, &testName}};
	ConstantPredicateMap predicateMap = ConstantPredicateMap(&m);

	Constant a = Constant("a");
	table.store(&a);
	Constant b = Constant("b");
	table.store(&b);
	Constant c = Constant("c");
	table.store(&c);

	ConstantsTable *filteredTable = table.filter(&predicateMap);

	// tableSize updated correctly
	REQUIRE(*table.retrieve("b") == Constant("b"));
	REQUIRE(filteredTable->getTableSize() == 1);
}

TEST_CASE("Successfully filters ProceduresTable using ConstantName") {
	ProceduresTable table;
	Procedure testName = Procedure("b"); 
	std::map<EntityHeader, Procedure*> m = {{EntityHeader::NAME, &testName}};
	ProcedurePredicateMap predicateMap = ProcedurePredicateMap(&m);

	Procedure a = Procedure("a");
	table.store(&a);
	Procedure b = Procedure("b");
	table.store(&b);
	Procedure c = Procedure("c");
	table.store(&c);

	ProceduresTable *filteredTable = table.filter(&predicateMap);

	// tableSize updated correctly
	REQUIRE(*table.retrieve("b") == Procedure("b"));
	REQUIRE(filteredTable->getTableSize() == 1);
}

TEST_CASE("Successfully filters VariablesTable using VariableName") {
	VariablesTable table;
	Variable testName = Variable("b"); 
	std::map<EntityHeader, Variable*> m = {{EntityHeader::NAME, &testName}};
	VariablePredicateMap predicateMap = VariablePredicateMap(&m);

	Variable a = Variable("a");
	table.store(&a);
	Variable b = Variable("b");
	table.store(&b);
	Variable c = Variable("c");
	table.store(&c);

	VariablesTable *filteredTable = table.filter(&predicateMap);

	// tableSize updated correctly
	REQUIRE(*table.retrieve("b") == Variable("b"));
	REQUIRE(filteredTable->getTableSize() == 1);
}
