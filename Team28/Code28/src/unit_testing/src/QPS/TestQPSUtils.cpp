#include "QPS/Utils.h"
#include "catch.hpp"
#include <vector>

TEST_CASE("Tokenizer can convert a query string into a vector of tokens") {
    // If pattern clause contains expressions with '(' or ')', the expression
    // will be split up into smaller tokens instead of remaining as one
    // expression.
    std::string query = "assign a; variable v; Select a such that Uses(a, v) "
                        "pattern a(_, _\"1\"_)";
    std::vector<char> specialChar{';', '(', ',', ')', '_'};
    std::vector<std::string> expected{
        "assign", "a",     ";",       "variable", "v", ";", "Select",
        "a",      "such",  "that",    "Uses",     "(", "a", ",",
        "v",      ")",     "pattern", "a",        "(", "_", ",",
        "_",      "\"1\"", "_",       ")"};

    std::vector<std::string> result = Utils::tokenize(query, specialChar);
    REQUIRE(result == expected);
}