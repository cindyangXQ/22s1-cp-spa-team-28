#include "QPS/Utils.h"
#include "catch.hpp"
#include <vector>

TEST_CASE("Tokenizer can convert a query string into a vector of tokens") {
    // If pattern clause contains expressions with '(' or ')', the expression
    // will be split up into smaller tokens instead of remaining as one
    // expression.
    std::string query = "assign a; variable v; Select a such that Uses(a, v) "
                        "pattern a(_, _\"1\"_)";
    std::vector<char> special_char{';', '(', ',', ')', '_'};
    std::vector<std::string> expected{
        "assign", "a",     ";",       "variable", "v", ";", "Select",
        "a",      "such",  "that",    "Uses",     "(", "a", ",",
        "v",      ")",     "pattern", "a",        "(", "_", ",",
        "_",      "\"1\"", "_",       ")"};

    std::vector<std::string> result = Utils::tokenize(query, special_char);
    REQUIRE(result == expected);
}

TEST_CASE("in function finds item in vector") {
    std::vector<std::string> v{"This", "is", "a", "test", ".", "1234"};
    std::string present = "test";
    std::string absent = "YAY";
    REQUIRE(Utils::in(v, present) == true);

    REQUIRE(Utils::in(v, absent) == false);
}
