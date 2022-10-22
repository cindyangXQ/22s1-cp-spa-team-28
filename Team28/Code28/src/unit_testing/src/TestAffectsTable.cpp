#include "PKB/Storage/Storage.h"
#include "PKB/Tables/RelationshipsTable/AffectsTable.h"
#include "commons/Relationship.h"

#include "catch.hpp"

struct InitAffectsTable {
public:
    static std::pair<AffectsTable *, StorageView *> initCode6();
};

std::pair<AffectsTable *, StorageView *> InitAffectsTable::initCode6() {
    // Refer to Tests28/source/Affects_code6_source.txt. Ignore First
    Storage *storage = new Storage();
    AffectsTable *affects = storage->getTable<AffectsTable>();
    NextTable *next = storage->getTable<NextTable>();
    NextTTable *nextT = storage->getTable<NextTTable>();
    UsesSTable *usesS = storage->getTable<UsesSTable>();
    ModifiesSTable *modS = storage->getTable<ModifiesSTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();

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

    Relationship<int, int> n1 = Relationship(RelationshipReference::NEXT, 2, 3);
    Relationship<int, int> n2 = Relationship(RelationshipReference::NEXT, 3, 4);
    Relationship<int, int> n3 = Relationship(RelationshipReference::NEXT, 3, 7);
    Relationship<int, int> n4 = Relationship(RelationshipReference::NEXT, 5, 6);
    Relationship<int, int> n5 = Relationship(RelationshipReference::NEXT, 7, 9);
    Relationship<int, int> n6 =
        Relationship(RelationshipReference::NEXT, 8, 10);
    std::vector<Relationship<int, int> *> nexts = {&n1, &n2, &n3,
                                                   &n4, &n5, &n6};
    for (Relationship<int, int> *n : nexts) {
        next->store(n);
    }

    Relationship<int, int> ns1 =
        Relationship(RelationshipReference::NEXT_T, 1, 2);
    Relationship<int, int> ns2 =
        Relationship(RelationshipReference::NEXT_T, 1, 3);
    Relationship<int, int> ns3 =
        Relationship(RelationshipReference::NEXT_T, 2, 5);
    Relationship<int, int> ns4 =
        Relationship(RelationshipReference::NEXT_T, 4, 3);
    Relationship<int, int> ns5 =
        Relationship(RelationshipReference::NEXT_T, 5, 5);
    Relationship<int, int> ns6 =
        Relationship(RelationshipReference::NEXT_T, 5, 8);
    Relationship<int, int> ns7 =
        Relationship(RelationshipReference::NEXT_T, 5, 12);
    std::vector<Relationship<int, int> *> nextTs = {&ns1, &ns2, &ns3, &ns4,
                                                    &ns5, &ns6, &ns7};
    for (Relationship<int, int> *ns : nextTs) {
        nextT->store(ns);
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
    std::vector<Relationship<int, std::string> *> mods = {
        &mod1, &mod2, &mod3,  &mod4,  &mod5,  &mod6, &mod7,
        &mod8, &mod9, &mod10, &mod11, &mod12, &mod13};
    for (Relationship<int, std::string> *modify : mods) {
        modS->store(modify);
    }

    affects->initAffects(storage->getStorageView());

    return std::make_pair(affects, storage->getStorageView());
}

TEST_CASE("AffectsTable: solveRight works correctly") {
    std::pair<AffectsTable *, StorageView *> pair =
        InitAffectsTable::initCode6();
    AffectsTable *affects = pair.first;
    StorageView *storage = pair.second;

    Reference leftRef = Reference("1");
    EntityName rightSynonym = EntityName::STMT;

    // Affects(1, s)
    std::vector<Value> output =
        affects->solveRight(leftRef, rightSynonym, storage);
    std::vector<Value> expectedResult = {
        Value(ValueType::STMT_NUM, "4"), Value(ValueType::STMT_NUM, "8"),
        Value(ValueType::STMT_NUM, "10"), Value(ValueType::STMT_NUM, "12")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects(1, a)
    rightSynonym = EntityName::ASSIGN;
    output = affects->solveRight(leftRef, rightSynonym, storage);
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

TEST_CASE("AffectsTable: solveLeft works correctly") {
    std::pair<AffectsTable *, StorageView *> pair =
        InitAffectsTable::initCode6();
    AffectsTable *affects = pair.first;
    StorageView *storage = pair.second;

    Reference rightRef = Reference("10");
    EntityName leftSynonym = EntityName::STMT;

    // Affects(s, 10)
    std::vector<Value> output =
        affects->solveLeft(rightRef, leftSynonym, storage);
    std::vector<Value> expectedResult = {
        Value(ValueType::STMT_NUM, "1"), Value(ValueType::STMT_NUM, "2"),
        Value(ValueType::STMT_NUM, "4"), Value(ValueType::STMT_NUM, "9")};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects(a, 10)
    leftSynonym = EntityName::ASSIGN;
    output = affects->solveLeft(rightRef, leftSynonym, storage);
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));

    // Affects(p, 10), empty
    leftSynonym = EntityName::PROCEDURE;
    output = affects->solveLeft(rightRef, leftSynonym, storage);
    expectedResult = {};
    std::sort(output.begin(), output.end());
    std::sort(expectedResult.begin(), expectedResult.end());
    REQUIRE(output.size() == expectedResult.size());
    REQUIRE(std::equal(expectedResult.begin(), expectedResult.end(),
                       output.begin()));
}
