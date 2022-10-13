#include "catch.hpp"

#include "PKB/Algorithms/ControlFlowGraph.h"
#include "PKB/Storage/Storage.h"

TEST_CASE("CFG Traverses Correctly - 1 procedure") {
    Storage *storage = new Storage();
    NextTable *nextTable = storage->getTable<NextTable>();
    FollowsTable *followsTable = storage->getTable<FollowsTable>();
    BranchInTable *branchIn = storage->getTable<BranchInTable>();
    BranchOutTable *branchOut = storage->getTable<BranchOutTable>();
    ProceduresTable *procTable = storage->getTable<ProceduresTable>();

    // Follows
    Relationship<int, int> relation =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 2, 3);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 4, 5);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 5, 6);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 3, 7);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 10, 11);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 11, 12);
    followsTable->store(&relation);

    // BranchIn
    relation = Relationship(RelationshipReference::NEXT, 3, 4);
    branchIn->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 7, 8);
    branchIn->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 7, 9);
    branchIn->store(&relation);

    // BranchOut
    relation = Relationship(RelationshipReference::NEXT, 6, 3);
    branchOut->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 8, 10);
    branchOut->store(&relation);
    relation = Relationship(RelationshipReference::NEXT, 9, 10);
    branchOut->store(&relation);

    // Procedure(s)
    Procedure test = Procedure("main", 1);
    procTable->store(&test);

    StorageView *storageView = storage->getStorageView();
    ControlFlowGraph cfg = ControlFlowGraph(nextTable, storageView);

    cfg.populateNext();

    REQUIRE(nextTable->validate(Reference("1"), Reference("2")));
    REQUIRE(nextTable->validate(Reference("2"), Reference("3")));
    REQUIRE(nextTable->validate(Reference("3"), Reference("4")));
    REQUIRE(nextTable->validate(Reference("4"), Reference("5")));
    REQUIRE(nextTable->validate(Reference("5"), Reference("6")));
    REQUIRE(nextTable->validate(Reference("6"), Reference("3")));
    REQUIRE(nextTable->validate(Reference("3"), Reference("7")));
    REQUIRE(nextTable->validate(Reference("7"), Reference("8")));
    REQUIRE(nextTable->validate(Reference("7"), Reference("9")));
    REQUIRE(nextTable->validate(Reference("8"), Reference("10")));
    REQUIRE(nextTable->validate(Reference("9"), Reference("10")));
    REQUIRE(nextTable->validate(Reference("10"), Reference("11")));
    REQUIRE(nextTable->validate(Reference("11"), Reference("12")));
}


TEST_CASE("CFG Traverses Correctly - multiple procedures") {
    Storage *storage = new Storage();
    NextTable *nextTable = storage->getTable<NextTable>();
    FollowsTable *followsTable = storage->getTable<FollowsTable>();
    BranchInTable *branchIn = storage->getTable<BranchInTable>();
    BranchOutTable *branchOut = storage->getTable<BranchOutTable>();
    ProceduresTable *procTable = storage->getTable<ProceduresTable>();

    // Follows
    Relationship<int, int> relation =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    followsTable->store(&relation);
    relation = Relationship(RelationshipReference::FOLLOWS, 5, 6);
    followsTable->store(&relation);

    // BranchIn
    relation = Relationship(RelationshipReference::NEXT, 3, 4);
    branchIn->store(&relation);

    // BranchOut
    relation = Relationship(RelationshipReference::NEXT, 4, 3);
    branchOut->store(&relation);

    // Procedure(s)
    Procedure main = Procedure("main", 1);
    procTable->store(&main);
    Procedure foo = Procedure("foo", 3);
    procTable->store(&foo);
    Procedure bar = Procedure("bar", 5);
    procTable->store(&bar);

    StorageView *storageView = storage->getStorageView();
    ControlFlowGraph cfg = ControlFlowGraph(nextTable, storageView);

    cfg.populateNext();

    REQUIRE(nextTable->validate(Reference("1"), Reference("2")));
    REQUIRE(nextTable->validate(Reference("3"), Reference("4")));
    REQUIRE(nextTable->validate(Reference("4"), Reference("3")));
    REQUIRE(nextTable->validate(Reference("5"), Reference("6")));
}
