#include "PKB/PopulateFacade.h"
#include "SP/SP.h"
#include "catch.hpp"

#include <iostream>
#include <vector>

std::string source1 = "../../../../Tests28/NoEntities.txt";
std::string source2 = "../../../../Tests28/AllEntities.txt";

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
