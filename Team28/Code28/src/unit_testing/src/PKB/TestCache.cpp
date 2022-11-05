#include "catch.hpp"

#include "PKB/Tables/Cache.h"

class CacheTable : public Cache {
    void resetCache() { return; }
};

TEST_CASE("Cache: Table initialised with isTableUsed to false") {
    CacheTable table = CacheTable();

    REQUIRE(table.shouldTableReset() == false);
}

TEST_CASE("Cache: markForReset works correctly") {
    CacheTable table = CacheTable();

    table.markForReset();
    REQUIRE(table.shouldTableReset() == true);
}

TEST_CASE("Cache: markTableResetted works correctly") {
    CacheTable table = CacheTable();

    table.markForReset();
    table.markTableResetted();
    REQUIRE(table.shouldTableReset() == false);
}