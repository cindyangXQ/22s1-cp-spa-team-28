#include "PKB/Facades/PopulateFacade.h"
#include "SP/SP.h"
#include "SP/ExtractUtils.h"
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

TEST_CASE("test expr converter") { 
    std::string expected = "((((a)+(b))+((c)/(d)))+((e)*(f)))";
    REQUIRE(ExtractUtils::convertExpression("a+b+c/d+e*f") == expected);
}
