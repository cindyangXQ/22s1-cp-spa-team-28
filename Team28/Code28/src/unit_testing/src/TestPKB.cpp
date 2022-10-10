#include "catch.hpp"

#include "PKB/PKB.h"

TEST_CASE("PKB is initialised correctly") {
    PKB pkb = PKB();

    // populateFacade and queryFacade are initialised
    REQUIRE(pkb.populateFacade != nullptr);
    REQUIRE(pkb.queryFacade != nullptr);
}

TEST_CASE("PKB::populateNext can populate NextTable") {
    PKB pkb = PKB();
    pkb.populateNext();
}