#include "catch.hpp"

#include "commons/Reference.h"

TEST_CASE("Reference isAttributable works correctly") {
    Reference callP =
        Reference(Synonym(EntityName::CALL, "c1"), EntityAttribute::PROC_NAME);
    Reference callS =
        Reference(Synonym(EntityName::CALL, "c2"), EntityAttribute::STMT_NO);
    Reference readV =
        Reference(Synonym(EntityName::READ, "r1"), EntityAttribute::VAR_NAME);
    Reference readS =
        Reference(Synonym(EntityName::READ, "r2"), EntityAttribute::STMT_NO);
    Reference printV =
        Reference(Synonym(EntityName::PRINT, "p1"), EntityAttribute::VAR_NAME);
    Reference printS =
        Reference(Synonym(EntityName::PRINT, "p2"), EntityAttribute::STMT_NO);

    REQUIRE(callP.isInferredAttribute());
    REQUIRE(!callS.isInferredAttribute());
    REQUIRE(readV.isInferredAttribute());
    REQUIRE(!readS.isInferredAttribute());
    REQUIRE(printV.isInferredAttribute());
    REQUIRE(!printS.isInferredAttribute());
}
