#include "catch.hpp"
#include "PKB/PopulateFacade.h"
#include "SP/SP.h"

#include <iostream>
#include <vector>

using namespace std;

TEST_CASE() {
	string source1 = "../../../../Tests28/NoEntities.txt";
	string source2 = "../../../../Tests28/AllEntities.txt";

	SP sp1 = SP(&PopulateFacade(Storage()));
	sp1.parse(source1);

	SP sp2 = SP(&PopulateFacade(Storage()));
	sp2.parse(source2);
}
