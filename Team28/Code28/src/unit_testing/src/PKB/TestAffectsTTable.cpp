#include "PKB/Algorithms/ControlFlowGraph.h"
#include "PKB/Storage/Storage.h"
#include "PKB/Tables/RelationshipsTable/AffectsTTable.h"
#include "PKB/Tables/RelationshipsTable/AffectsTable.h"
#include "commons/Relationship.h"

#include "catch.hpp"

struct InitAffectsTTable {
public:
    static std::pair<AffectsTTable *, StorageView *> initCode6();
};

std::pair<AffectsTTable *, StorageView *> InitAffectsTTable::initCode6() {
    // Refer to Tests28/source/Affects_code6_source.txt. Ignore First
    Storage *storage = new Storage();
    AffectsTable *affects = storage->getTable<AffectsTable>();
    AffectsTTable *affectsT = storage->getTable<AffectsTTable>();
    NextTable *next = storage->getTable<NextTable>();
    UsesSTable *usesS = storage->getTable<UsesSTable>();
    ModifiesSTable *modS = storage->getTable<ModifiesSTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    ProceduresTable *procedures = storage->getTable<ProceduresTable>();
    FollowsTable *follows = storage->getTable<FollowsTable>();
    BranchInTable *branchIn = storage->getTable<BranchInTable>();
    BranchOutTable *branchOut = storage->getTable<BranchOutTable>();

    Statement s1 = Statement(1, StatementType::ASSIGN);
    Statement s2 = Statement(2, StatementType::ASSIGN);
    Statement s3 = Statement(3, StatementType::WHILE);
    Statement s4 = Statement(4, StatementType::ASSIGN);
    Statement s5 = Statement(5, StatementType::CALL);
    Statement s6 = Statement(6, StatementType::ASSIGN);
    Statement s7 = Statement(7, StatementType::IF);
    Statement s8 = Statement(8, StatementType::ASSIGN);
    Statement s9 = Statement(9, StatementType::ASSIGN);
    Statement s10 = Statement(10, StatementType::ASSIGN);
    Statement s11 = Statement(11, StatementType::ASSIGN);
    Statement s12 = Statement(12, StatementType::ASSIGN);
    Statement s13 = Statement(13, StatementType::ASSIGN);
    Statement s14 = Statement(14, StatementType::ASSIGN);
    Statement s15 = Statement(15, StatementType::PRINT);
    std::vector<Statement *> stmts = {&s1,  &s2,  &s3,  &s4,  &s5,
                                      &s6,  &s7,  &s8,  &s9,  &s10,
                                      &s11, &s12, &s13, &s14, &s15};
    for (Statement *stmt : stmts) {
        statements->store(stmt);
    }

    Procedure p1 = Procedure("Second", 1);
    Procedure p2 = Procedure("Third", 13);
    std::vector<Procedure *> procs = {&p1, &p2};
    for (Procedure *p : procs) {
        procedures->store(p);
    }

    Relationship<int, int> f1 =
        Relationship(RelationshipReference::FOLLOWS, 1, 2);
    Relationship<int, int> f2 =
        Relationship(RelationshipReference::FOLLOWS, 2, 3);
    Relationship<int, int> f3 =
        Relationship(RelationshipReference::FOLLOWS, 3, 7);
    Relationship<int, int> f4 =
        Relationship(RelationshipReference::FOLLOWS, 4, 5);
    Relationship<int, int> f5 =
        Relationship(RelationshipReference::FOLLOWS, 5, 6);
    Relationship<int, int> f6 =
        Relationship(RelationshipReference::FOLLOWS, 7, 10);
    Relationship<int, int> f7 =
        Relationship(RelationshipReference::FOLLOWS, 10, 11);
    Relationship<int, int> f8 =
        Relationship(RelationshipReference::FOLLOWS, 11, 12);
    Relationship<int, int> f9 =
        Relationship(RelationshipReference::FOLLOWS, 13, 14);
    Relationship<int, int> f10 =
        Relationship(RelationshipReference::FOLLOWS, 14, 15);
    std::vector<Relationship<int, int> *> fs = {&f1, &f2, &f3, &f4, &f5,
                                                &f6, &f7, &f8, &f9, &f10};
    for (Relationship<int, int> *f : fs) {
        follows->store(f);
    }

    Relationship<int, int> bi1 =
        Relationship(RelationshipReference::NEXT, 3, 4);
    Relationship<int, int> bi2 =
        Relationship(RelationshipReference::NEXT, 7, 8);
    Relationship<int, int> bi3 =
        Relationship(RelationshipReference::NEXT, 7, 9);
    std::vector<Relationship<int, int> *> bis = {&bi1, &bi2, &bi3};
    for (Relationship<int, int> *bi : bis) {
        branchIn->store(bi);
    }

    Relationship<int, int> bo1 =
        Relationship(RelationshipReference::NEXT, 6, 3);
    Relationship<int, int> bo2 =
        Relationship(RelationshipReference::NEXT, 8, 10);
    Relationship<int, int> bo3 =
        Relationship(RelationshipReference::NEXT, 9, 10);
    std::vector<Relationship<int, int> *> bos = {&bo1, &bo2, &bo3};
    for (Relationship<int, int> *bo : bos) {
        branchOut->store(bo);
    }

    Relationship<int, std::string> use1 =
        Relationship(RelationshipReference::USES, 3, std::string("i"));
    Relationship<int, std::string> use2 =
        Relationship(RelationshipReference::USES, 4, std::string("x"));
    Relationship<int, std::string> use3 =
        Relationship(RelationshipReference::USES, 4, std::string("y"));
    Relationship<int, std::string> use4 =
        Relationship(RelationshipReference::USES, 6, std::string("i"));
    Relationship<int, std::string> use5 =
        Relationship(RelationshipReference::USES, 7, std::string("x"));
    Relationship<int, std::string> use6 =
        Relationship(RelationshipReference::USES, 8, std::string("x"));
    Relationship<int, std::string> use7 =
        Relationship(RelationshipReference::USES, 10, std::string("z"));
    Relationship<int, std::string> use8 =
        Relationship(RelationshipReference::USES, 10, std::string("x"));
    Relationship<int, std::string> use9 =
        Relationship(RelationshipReference::USES, 10, std::string("i"));
    Relationship<int, std::string> use10 =
        Relationship(RelationshipReference::USES, 11, std::string("z"));
    Relationship<int, std::string> use11 =
        Relationship(RelationshipReference::USES, 12, std::string("x"));
    Relationship<int, std::string> use12 =
        Relationship(RelationshipReference::USES, 12, std::string("y"));
    Relationship<int, std::string> use13 =
        Relationship(RelationshipReference::USES, 12, std::string("z"));
    Relationship<int, std::string> use14 =
        Relationship(RelationshipReference::USES, 14, std::string("z"));
    Relationship<int, std::string> use15 =
        Relationship(RelationshipReference::USES, 15, std::string("v"));
    std::vector<Relationship<int, std::string> *> uses = {
        &use1, &use2,  &use3,  &use4,  &use5,  &use6,  &use7, &use8,
        &use9, &use10, &use11, &use12, &use13, &use14, &use15};
    for (Relationship<int, std::string> *use : uses) {
        usesS->store(use);
    }

    Relationship<int, std::string> mod1 =
        Relationship(RelationshipReference::MODIFIES, 1, std::string("x"));
    Relationship<int, std::string> mod2 =
        Relationship(RelationshipReference::MODIFIES, 2, std::string("i"));
    Relationship<int, std::string> mod3 =
        Relationship(RelationshipReference::MODIFIES, 3, std::string("i"));
    Relationship<int, std::string> mod4 =
        Relationship(RelationshipReference::MODIFIES, 4, std::string("x"));
    Relationship<int, std::string> mod5 =
        Relationship(RelationshipReference::MODIFIES, 6, std::string("i"));
    Relationship<int, std::string> mod6 =
        Relationship(RelationshipReference::MODIFIES, 7, std::string("x"));
    Relationship<int, std::string> mod7 =
        Relationship(RelationshipReference::MODIFIES, 8, std::string("x"));
    Relationship<int, std::string> mod8 =
        Relationship(RelationshipReference::MODIFIES, 9, std::string("z"));
    Relationship<int, std::string> mod9 =
        Relationship(RelationshipReference::MODIFIES, 10, std::string("z"));
    Relationship<int, std::string> mod10 =
        Relationship(RelationshipReference::MODIFIES, 11, std::string("y"));
    Relationship<int, std::string> mod11 =
        Relationship(RelationshipReference::MODIFIES, 12, std::string("x"));
    Relationship<int, std::string> mod12 =
        Relationship(RelationshipReference::MODIFIES, 13, std::string("z"));
    Relationship<int, std::string> mod13 =
        Relationship(RelationshipReference::MODIFIES, 14, std::string("v"));
    Relationship<int, std::string> mod14 =
        Relationship(RelationshipReference::MODIFIES, 5, std::string("z"));
    Relationship<int, std::string> mod15 =
        Relationship(RelationshipReference::MODIFIES, 5, std::string("v"));
    Relationship<int, std::string> mod16 =
        Relationship(RelationshipReference::MODIFIES, 7, std::string("z"));
    Relationship<int, std::string> mod17 =
        Relationship(RelationshipReference::MODIFIES, 3, std::string("x"));
    Relationship<int, std::string> mod18 =
        Relationship(RelationshipReference::MODIFIES, 3, std::string("y"));
    std::vector<Relationship<int, std::string> *> mods = {
        &mod1,  &mod2,  &mod3,  &mod4,  &mod5,  &mod6,  &mod7,  &mod8,  &mod9,
        &mod10, &mod11, &mod12, &mod13, &mod14, &mod15, &mod16, &mod17, &mod18};
    for (Relationship<int, std::string> *modify : mods) {
        modS->store(modify);
    }

    ControlFlowGraph cfg = ControlFlowGraph(next, storage->getStorageView());
    cfg.populateNext();
    affects->initAffects(storage->getStorageView());
    affects->resetCache();
    affectsT->initAffectsT(storage->getStorageView());
    affectsT->resetCache();

    return std::make_pair(affectsT, storage->getStorageView());
}

TEST_CASE("AffectsTTable: validate works correctly") {
    std::pair<AffectsTTable *, StorageView *> pair =
        InitAffectsTTable::initCode6();
    AffectsTTable *affects = pair.first;
    StorageView *storage = pair.second;

    // Affects
    REQUIRE(affects->validate(Reference("1"), Reference("4")));
    REQUIRE(affects->validate(Reference("1"), Reference("8")));
    REQUIRE(affects->validate(Reference("1"), Reference("10")));
    REQUIRE(affects->validate(Reference("1"), Reference("12")));
    REQUIRE(affects->validate(Reference("2"), Reference("6")));
    REQUIRE(affects->validate(Reference("2"), Reference("10")));
    REQUIRE(affects->validate(Reference("4"), Reference("4")));
    REQUIRE(affects->validate(Reference("4"), Reference("8")));
    REQUIRE(affects->validate(Reference("4"), Reference("10")));
    REQUIRE(affects->validate(Reference("4"), Reference("12")));
    REQUIRE(affects->validate(Reference("6"), Reference("6")));
    REQUIRE(affects->validate(Reference("6"), Reference("10")));
    REQUIRE(affects->validate(Reference("8"), Reference("10")));
    REQUIRE(affects->validate(Reference("8"), Reference("12")));
    REQUIRE(affects->validate(Reference("9"), Reference("10")));
    REQUIRE(affects->validate(Reference("10"), Reference("11")));
    REQUIRE(affects->validate(Reference("10"), Reference("12")));
    REQUIRE(affects->validate(Reference("11"), Reference("12")));
    REQUIRE(affects->validate(Reference("13"), Reference("14")));
    // Affects*
    REQUIRE(affects->validate(Reference("2"), Reference("11")));
    REQUIRE(affects->validate(Reference("2"), Reference("12")));
    REQUIRE(affects->validate(Reference("4"), Reference("11")));
    REQUIRE(affects->validate(Reference("6"), Reference("11")));
    REQUIRE(affects->validate(Reference("6"), Reference("12")));
    REQUIRE(affects->validate(Reference("8"), Reference("11")));
    REQUIRE(affects->validate(Reference("9"), Reference("11")));
    REQUIRE(affects->validate(Reference("9"), Reference("12")));

    REQUIRE(affects->validate(Reference("_"), Reference("_")));
    REQUIRE(affects->validate(Reference("2"), Reference("_")));
    REQUIRE(affects->validate(Reference("_"), Reference("10")));
    REQUIRE(!affects->validate(Reference("_"), Reference("15")));
    REQUIRE(!affects->validate(Reference("15"), Reference("_")));
}

TEST_CASE("AffectsTTable: solveRight works correctly") {
    std::pair<AffectsTTable *, StorageView *> pair =
        InitAffectsTTable::initCode6();
    AffectsTTable *affects = pair.first;
    StorageView *storage = pair.second;

    Reference leftRef = Reference("1");
    EntityName rightSynonym = EntityName::STMT;

    // Affects*(1, s)
    std::vector<Value> output =
        affects->solveRight(leftRef, rightSynonym, storage);
    std::vector<Value> expectedResult = {
        Value(ValueType::STMT_NUM, "4"), Value(ValueType::STMT_NUM, "8"),
        Value(ValueType::STMT_NUM, "10"), Value(ValueType::STMT_NUM, "11"),
        Value(ValueType::STMT_NUM, "12")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(1, a)
    rightSynonym = EntityName::ASSIGN;
    output = affects->solveRight(leftRef, rightSynonym, storage);
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(_, a)
    leftRef = Reference("_");
    rightSynonym = EntityName::ASSIGN;
    output = affects->solveRight(leftRef, rightSynonym, storage);
    expectedResult = {
        Value(ValueType::STMT_NUM, "4"),  Value(ValueType::STMT_NUM, "6"),
        Value(ValueType::STMT_NUM, "8"),  Value(ValueType::STMT_NUM, "10"),
        Value(ValueType::STMT_NUM, "11"), Value(ValueType::STMT_NUM, "12"),
        Value(ValueType::STMT_NUM, "14")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects(1, p), empty
    rightSynonym = EntityName::PROCEDURE;
    output = affects->solveRight(leftRef, rightSynonym, storage);
    expectedResult = {};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AffectsTTable: solveLeft works correctly") {
    std::pair<AffectsTTable *, StorageView *> pair =
        InitAffectsTTable::initCode6();
    AffectsTTable *affects = pair.first;
    StorageView *storage = pair.second;

    Reference rightRef = Reference("10");
    EntityName leftSynonym = EntityName::STMT;

    // Affects(s, 10)
    std::vector<Value> output =
        affects->solveLeft(rightRef, leftSynonym, storage);
    std::vector<Value> expectedResult = {
        Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "2"),
        Value(ValueType::STMT_NUM, "4"), Value(ValueType::STMT_NUM, "6"),
        Value(ValueType::STMT_NUM, "8"), Value(ValueType::STMT_NUM, "9")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());

    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(a, 10)
    leftSynonym = EntityName::ASSIGN;
    output = affects->solveLeft(rightRef, leftSynonym, storage);
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(a, _)
    leftSynonym = EntityName::ASSIGN;
    rightRef = Reference("_");
    output = affects->solveLeft(rightRef, leftSynonym, storage);
    expectedResult = {
        Value(ValueType::STMT_NUM, "1"),  Value(ValueType::STMT_NUM, "2"),
        Value(ValueType::STMT_NUM, "4"),  Value(ValueType::STMT_NUM, "6"),
        Value(ValueType::STMT_NUM, "8"),  Value(ValueType::STMT_NUM, "9"),
        Value(ValueType::STMT_NUM, "10"), Value(ValueType::STMT_NUM, "11"),
        Value(ValueType::STMT_NUM, "13")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(p, 10), empty
    leftSynonym = EntityName::PROCEDURE;
    output = affects->solveLeft(rightRef, leftSynonym, storage);
    expectedResult = {};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AffectsTTable:solveBoth works correctly") {
    std::pair<AffectsTTable *, StorageView *> pair =
        InitAffectsTTable::initCode6();
    AffectsTTable *affects = pair.first;
    StorageView *storage = pair.second;

    EntityName leftSynonym = EntityName::STMT;
    EntityName rightSynonym = EntityName::STMT;

    // Affects(s1, s2)
    std::vector<std::pair<Value, Value>> output =
        affects->solveBoth(leftSynonym, rightSynonym, storage);
    std::vector<std::pair<Value, Value>> expectedResult = {
        std::make_pair(Value(ValueType::STMT_NUM, "2"),
                       Value(ValueType::STMT_NUM, "6")),
        std::make_pair(Value(ValueType::STMT_NUM, "4"),
                       Value(ValueType::STMT_NUM, "8")),
        std::make_pair(Value(ValueType::STMT_NUM, "4"),
                       Value(ValueType::STMT_NUM, "10")),
        std::make_pair(Value(ValueType::STMT_NUM, "6"),
                       Value(ValueType::STMT_NUM, "6")),
        std::make_pair(Value(ValueType::STMT_NUM, "1"),
                       Value(ValueType::STMT_NUM, "4")),
        std::make_pair(Value(ValueType::STMT_NUM, "1"),
                       Value(ValueType::STMT_NUM, "8")),
        std::make_pair(Value(ValueType::STMT_NUM, "1"),
                       Value(ValueType::STMT_NUM, "10")),
        std::make_pair(Value(ValueType::STMT_NUM, "1"),
                       Value(ValueType::STMT_NUM, "12")),
        std::make_pair(Value(ValueType::STMT_NUM, "2"),
                       Value(ValueType::STMT_NUM, "10")),
        std::make_pair(Value(ValueType::STMT_NUM, "9"),
                       Value(ValueType::STMT_NUM, "10")),
        std::make_pair(Value(ValueType::STMT_NUM, "6"),
                       Value(ValueType::STMT_NUM, "10")),
        std::make_pair(Value(ValueType::STMT_NUM, "8"),
                       Value(ValueType::STMT_NUM, "10")),
        std::make_pair(Value(ValueType::STMT_NUM, "8"),
                       Value(ValueType::STMT_NUM, "12")),
        std::make_pair(Value(ValueType::STMT_NUM, "10"),
                       Value(ValueType::STMT_NUM, "11")),
        std::make_pair(Value(ValueType::STMT_NUM, "10"),
                       Value(ValueType::STMT_NUM, "12")),
        std::make_pair(Value(ValueType::STMT_NUM, "11"),
                       Value(ValueType::STMT_NUM, "12")),
        std::make_pair(Value(ValueType::STMT_NUM, "13"),
                       Value(ValueType::STMT_NUM, "14")),
        std::make_pair(Value(ValueType::STMT_NUM, "4"),
                       Value(ValueType::STMT_NUM, "4")),
        std::make_pair(Value(ValueType::STMT_NUM, "4"),
                       Value(ValueType::STMT_NUM, "12")),
        std::make_pair(Value(ValueType::STMT_NUM, "1"),
                       Value(ValueType::STMT_NUM, "11")),
        std::make_pair(Value(ValueType::STMT_NUM, "2"),
                       Value(ValueType::STMT_NUM, "11")),
        std::make_pair(Value(ValueType::STMT_NUM, "2"),
                       Value(ValueType::STMT_NUM, "12")),
        std::make_pair(Value(ValueType::STMT_NUM, "4"),
                       Value(ValueType::STMT_NUM, "11")),
        std::make_pair(Value(ValueType::STMT_NUM, "6"),
                       Value(ValueType::STMT_NUM, "11")),
        std::make_pair(Value(ValueType::STMT_NUM, "6"),
                       Value(ValueType::STMT_NUM, "12")),
        std::make_pair(Value(ValueType::STMT_NUM, "8"),
                       Value(ValueType::STMT_NUM, "11")),
        std::make_pair(Value(ValueType::STMT_NUM, "9"),
                       Value(ValueType::STMT_NUM, "11")),
        std::make_pair(Value(ValueType::STMT_NUM, "9"),
                       Value(ValueType::STMT_NUM, "12")),
    };
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(a1, a2), equiv. to Affects*(s1, s2)
    leftSynonym = EntityName::ASSIGN;
    rightSynonym = EntityName::ASSIGN;
    output = affects->solveBoth(leftSynonym, rightSynonym, storage);
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(a1, p), empty
    leftSynonym = EntityName::ASSIGN;
    rightSynonym = EntityName::PROCEDURE;
    output = affects->solveBoth(leftSynonym, rightSynonym, storage);
    expectedResult = {};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(p1, p2), empty
    leftSynonym = EntityName::PROCEDURE;
    rightSynonym = EntityName::PROCEDURE;
    output = affects->solveBoth(leftSynonym, rightSynonym, storage);
    expectedResult = {};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AffectsTTable: solveBothReflexive works correctly") {
    std::pair<AffectsTTable *, StorageView *> pair =
        InitAffectsTTable::initCode6();
    AffectsTTable *affects = pair.first;
    StorageView *storage = pair.second;

    EntityName synonym = EntityName::STMT;

    // Affects*(s, s)
    std::vector<Value> output = affects->solveBothReflexive(synonym, storage);
    std::vector<Value> expectedResult = {Value(ValueType::STMT_NUM, "4"),
                                         Value(ValueType::STMT_NUM, "6")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(a, a), equiv. to Affects*(s, s)
    synonym = EntityName::ASSIGN;
    output = affects->solveBothReflexive(synonym, storage);
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects*(p, p), empty
    synonym = EntityName::PROCEDURE;
    output = affects->solveBothReflexive(synonym, storage);
    expectedResult = {};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}

TEST_CASE("AffectsTTable: getTableSize works correctly") {
    AffectsTTable affectsTTable;

    REQUIRE(affectsTTable.getTableSize() == INT_MAX);
}