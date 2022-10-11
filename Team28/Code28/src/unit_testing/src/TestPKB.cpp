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
    Relationship<int, int> test1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Relationship<int, int> test2 =
        Relationship(RelationshipReference::FOLLOWS, 2, 3);
    Relationship<int, int> test3 =
        Relationship(RelationshipReference::FOLLOWS, 3, 4);
    std::vector<Relationship<int, int> *> follows = {&test1, &test2, &test3};
    pkb.populateFacade->storeFollows(&follows);

    pkb.populateNext();

    REQUIRE(pkb.queryFacade->validate(RelationshipReference::NEXT,
                                      Reference("1"), Reference("2")));
    REQUIRE(pkb.queryFacade->validate(RelationshipReference::NEXT,
                                      Reference("2"), Reference("3")));
    REQUIRE(pkb.queryFacade->validate(RelationshipReference::NEXT,
                                      Reference("3"), Reference("4")));
}
