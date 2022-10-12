#include "QPS/Evaluator/ClauseTable.h"
#include "QPS/Evaluator/Tuple.h"
#include "commons/Synonym.h"
#include <vector>

#include "catch.hpp"

TEST_CASE("ClauseTable is initialised correctly") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    std::vector<Reference> refs{Reference(syns[0]), Reference(syns[1])};                    
    ClauseTable table = ClauseTable(refs);
    REQUIRE(table.size() == 0);
}

TEST_CASE("ClauseTable can insert tuple") {
    std::vector<Synonym> syns{Synonym(EntityName::VARIABLE, "v"),
                              Synonym(EntityName::ASSIGN, "a")};
    std::vector<Reference> refs{Reference(syns[0]), Reference(syns[1])};                    
    ClauseTable table = ClauseTable(refs);
    REQUIRE(table.size() == 0);
    table.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "foo"),
                                   Value(ValueType::STMT_NUM, "2")}));
    REQUIRE(table.size() == 1);
    table.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "yey"),
                                   Value(ValueType::STMT_NUM, "5")}));
    REQUIRE(table.size() == 2);
}

TEST_CASE("ClauseTable can get common headers from two tables") {
    Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
    Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
    Synonym syn3 = Synonym(EntityName::STMT, "s");
    std::vector<Reference> header1{Reference(syn1), Reference(syn2)};
    std::vector<Reference> header2{Reference(syn2), Reference(syn3)};
    ClauseTable table1 = ClauseTable(header1);
    ClauseTable table2 = ClauseTable(header2);
    std::vector<Reference> common_headers =
        ClauseTable::getCommonHeaders(table1, table2);
    REQUIRE(common_headers.size() == 1);
}

TEST_CASE("ClauseTable can get indices of common headers from a tables") {
    Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
    Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
    Synonym syn3 = Synonym(EntityName::STMT, "s");

    std::vector<Reference> header1{Reference(syn1), Reference(syn2), Reference(syn3)};
    std::vector<Reference> common_header{Reference(syn3), Reference(syn1)};

    ClauseTable table1 = ClauseTable(header1);
    std::vector<int> indices = table1.getIndices(common_header);
    REQUIRE(indices.size() == 2);
    REQUIRE(indices[0] == 2);
    REQUIRE(indices[1] == 0);
}

TEST_CASE("ClauseTable can create new table from two input table") {
    Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
    Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
    Synonym syn3 = Synonym(EntityName::STMT, "s");
    std::vector<Reference> header1{Reference(syn1), Reference(syn2)};
    std::vector<Reference> header2{Reference(syn2), Reference(syn3)};
    ClauseTable table1 = ClauseTable(header1);
    ClauseTable table2 = ClauseTable(header2);
    ClauseTable tableJoin = ClauseTable::ConstructTable(table1, table2);
    REQUIRE(tableJoin.header.size() == 3);
    REQUIRE(tableJoin.header[0].syn.name == syn2.name);
    REQUIRE(tableJoin.header[1].syn.name == syn1.name);
    REQUIRE(tableJoin.header[2].syn.name == syn3.name);
}

TEST_CASE("ClauseTable can join two tables with common headers") {
    Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
    Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
    Synonym syn3 = Synonym(EntityName::STMT, "s");
    std::vector<Reference> header1{Reference(syn1), Reference(syn2)};
    std::vector<Reference> header2{Reference(syn3), Reference(syn2)};
    ClauseTable table1 = ClauseTable(header1);
    ClauseTable table2 = ClauseTable(header2);
    table1.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "foo"),
                                    Value(ValueType::STMT_NUM, "2")}));
    table1.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "bar"),
                                    Value(ValueType::STMT_NUM, "3")}));
    table1.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "baz"),
                                    Value(ValueType::STMT_NUM, "4")}));
    table2.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "yey"),
                                    Value(ValueType::STMT_NUM, "3")}));
    table2.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "lol"),
                                    Value(ValueType::STMT_NUM, "4")}));
    table2.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "wtf"),
                                    Value(ValueType::STMT_NUM, "5")}));
    ClauseTable result = ClauseTable::joinTables(table1, table2);
    REQUIRE(result.size() == 2);
    REQUIRE(result.header.size() == 3);
    REQUIRE(result.header[0].syn.name == syn2.name);
    REQUIRE(result.header[1].syn.name == syn1.name);
    REQUIRE(result.header[2].syn.name == syn3.name);
}

TEST_CASE("ClauseTable can join two tables with no common headers") {
    Synonym syn1 = Synonym(EntityName::VARIABLE, "v");
    Synonym syn2 = Synonym(EntityName::ASSIGN, "a");
    Synonym syn3 = Synonym(EntityName::STMT, "s");
    Synonym syn4 = Synonym(EntityName::STMT, "s1");
    std::vector<Reference> header1{Reference(syn1), Reference(syn2)};
    std::vector<Reference> header2{Reference(syn3), Reference(syn4)};
    ClauseTable table1 = ClauseTable(header1);
    ClauseTable table2 = ClauseTable(header2);
    table1.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "foo"),
                                    Value(ValueType::STMT_NUM, "2")}));
    table1.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "bar"),
                                    Value(ValueType::STMT_NUM, "3")}));
    table1.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "baz"),
                                    Value(ValueType::STMT_NUM, "4")}));
    table2.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "yey"),
                                    Value(ValueType::STMT_NUM, "3")}));
    table2.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "lol"),
                                    Value(ValueType::STMT_NUM, "4")}));
    table2.insert(Tuple(std::vector{Value(ValueType::VAR_NAME, "wtf"),
                                    Value(ValueType::STMT_NUM, "5")}));
    ClauseTable result = ClauseTable::joinTables(table1, table2);
    REQUIRE(result.size() == 9);
    REQUIRE(result.header.size() == 4);
    REQUIRE(result.header[0].syn.name == syn1.name);
    REQUIRE(result.header[1].syn.name == syn2.name);
    REQUIRE(result.header[2].syn.name == syn3.name);
    REQUIRE(result.header[3].syn.name == syn4.name);
}
