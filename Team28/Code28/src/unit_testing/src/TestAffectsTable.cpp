#include "PKB/Storage/Storage.h"
#include "PKB/Tables/RelationshipsTable/AffectsTable.h"
#include "commons/Relationship.h"

#include "catch.hpp"

struct InitAffectsTable {
public:
    AffectsTable *initCode6();
};

AffectsTable *InitAffectsTable::initCode6() {
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
}
