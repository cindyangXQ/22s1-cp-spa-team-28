#include "PKB/Facades/PopulateFacade.h"
#include "SP/SP.h"
#include "catch.hpp"

#include <iostream>
#include <vector>

std::string source1 = "../../../../Tests28/NoEntities.txt";
std::string source2 = "../../../../Tests28/AllEntities.txt";

TEST_CASE() {
    Storage *storage = new Storage();
    PopulateFacade facade(storage);
    SP sp(&facade);
    sp.parse(source1);
}

TEST_CASE() {
    Storage *storage = new Storage();
    PopulateFacade facade(storage);
    SP sp(&facade);
    sp.parse(source2);
}

TEST_CASE("test expr converter") {
    std::string expected = "((((a)+(b))+((c)/(d)))+((e)*(f)))";
    REQUIRE(SP::convertExpression("a+b+c/d+e*f") == expected);
}

TEST_CASE("expression with wrong syntax in expr converter") {
    REQUIRE_THROWS(SP::convertExpression(")"), "expression syntax error");
    REQUIRE_THROWS(SP::convertExpression("x)"), "expression syntax error");
    REQUIRE_THROWS(SP::convertExpression("(x+1)*8+(y*9)-"),
                   "expression syntax error");
}
