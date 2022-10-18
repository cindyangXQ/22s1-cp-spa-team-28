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

    REQUIRE(callP.isSecondaryAttribute());
    REQUIRE(!callS.isSecondaryAttribute());
    REQUIRE(readV.isSecondaryAttribute());
    REQUIRE(!readS.isSecondaryAttribute());
    REQUIRE(printV.isSecondaryAttribute());
    REQUIRE(!printS.isSecondaryAttribute());
}
