#include "catch.hpp"

#include "../../spa/src/SPA.h"

TEST_CASE("SPA can parse entities correctly") {
    SPA spa;
    std::string filename =
        "../../../../../../Tests28/source/AllEntities_source.txt";

    spa.parse(filename);

    PKB pkb = spa.getPKB();
    std::vector<std::string> procs = pkb.queryFacade->getAllProcedures();
    std::vector<std::string> expectedProcs = {"foo", "bar", "foobar"};
    std::vector<std::string> consts = pkb.queryFacade->getAllConstants();
    std::vector<std::string> expectedConsts = {"1", "2", "3"};
    std::vector<std::string> vars = pkb.queryFacade->getAllVariables();
    std::vector<Statement *> statements = pkb.queryFacade->getAllStatements();
    std::vector<int> statementLines;
    std::transform(statements.begin(), statements.end(),
                   std::back_inserter(statementLines),
                   [](Statement *s) { return s->getLineNumber(); });
    std::vector<int> expectedStatementLines = {1, 2, 3, 4, 5, 6, 7, 8};

    REQUIRE(procs.size() == 3);
    REQUIRE(consts.size() == 3);
    REQUIRE(vars.size() == 7);
    REQUIRE(statements.size() == 8);

    for (std::string proc : expectedProcs) {
        REQUIRE(std::find(procs.begin(), procs.end(), proc) != procs.end());
    }

    for (std::string constant : expectedConsts) {
        REQUIRE(std::find(consts.begin(), consts.end(), constant) !=
                consts.end());
    }

    for (int lineNo : expectedStatementLines) {
        REQUIRE(std::find(statementLines.begin(), statementLines.end(),
                          lineNo) != statementLines.end());
    }
}
