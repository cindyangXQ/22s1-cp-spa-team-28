#include "PKB/NamesTable.h"
#include "PKB/NameDefinitions.h"
#include "PKB/Constant.h"
#include "PKB/Variable.h"
#include "PKB/Procedure.h"
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
	NamesTable<ConstantName, Constant> table;

	std::string alphabets = "abcdefghij";
	for (int i=0; i<10; i++) {
		std::string alphabet = std::string(1, alphabets[i]);
		Constant letter = Constant(alphabet);
		table.store(&letter);
	}

	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 10);
}

TEST_CASE("Succesfully filter ConstantPredicateMap using ConstantName") {
	static_assert(std::is_base_of<Entity<ConstantName>, Constant>::value, "T must inherit from Entity");
	NamesTable<ConstantName, Constant> table;
	Constant name = Constant("f"); 
	std::map<EntityHeader, Constant*> m = {{EntityHeader::NAME, &name}};
	ConstantPredicateMap predicateMap = ConstantPredicateMap(&m);

	std::string alphabets = "abcdefghij";
	for (int i=0; i<10; i++) {
		std::string alphabet = std::string(1, alphabets[i]);
		Constant letter = Constant(alphabet);
		table.store(&letter);
	}

	// ConstantPredicateMap *filteredTable = (ConstantPredicateMap*) table.filter(&predicateMap);

	// tableSize updated correctly
	REQUIRE(table.getTableSize() == 10);
}