#include "catch.hpp"
#include "PKB/PopulateFacade.h"
#include "SP/SP.h"

#include <iostream>
#include <vector>

string source1 = "../../../../Tests28/NoEntities.txt";
string source2 = "../../../../Tests28/AllEntities.txt";

TEST_CASE() {
	Storage storage;
	PopulateFacade facade(&storage);
	SP sp(&facade);
	sp.parse(source1);
}

TEST_CASE() {
	Storage storage;
	PopulateFacade facade(&storage);
	SP sp(&facade);
	sp.parse(source2);
}
