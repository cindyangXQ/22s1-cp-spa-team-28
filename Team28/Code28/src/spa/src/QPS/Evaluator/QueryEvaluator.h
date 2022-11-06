#pragma once

#include "../../PKB/Facades/QueryFacade.h"
#include "../Clause/PatternClause.h"
#include "../Clause/SelectClause.h"
#include "../Clause/SuchThatClause.h"
#include "../Clause/WithClause.h"
#include "../Error/EmptyTableError.h"
#include "../Parser/SolvableQuery.h"
#include "../Utils.h"
#include "QueryResult.h"
#include <string>

class QueryEvaluator {
private:
    QueryFacade *queryFacade;

public:
    explicit QueryEvaluator(QueryFacade *queryFacade)
        : queryFacade(queryFacade){};

    /*
     * Evaluate a query after it is parsed.
     */
    QueryResult evaluate(SolvableQuery *solvableQ);

    /*
     * Return the final results after all the clauses are evaluated.
     */
    std::vector<std::string> interpretQueryResult(QueryResult *queryResult);

private:
    void checkAllClauseResult(
        std::vector<std::vector<ClauseResult>> clauseResultList,
        bool *isAnyTableEmpty, bool *haveTableToJoin);
    std::vector<std::string> extractReferenceFromTable(Reference selectedRef,
                                                       ClauseTable result);
    std::vector<std::string>
    extractTuplesFromTable(std::vector<Reference> selectRefs,
                           ClauseTable result);
    std::vector<std::string> getAll(Reference select);
    std::vector<std::string> handleNoTables(QueryResult *queryResult);
    ClauseTable joinAllClauseTables(
        std::vector<std::vector<ClauseResult>> clauseResultList);
    std::string getAttributeValue(Reference ref, std::string synonymValue);
    bool isAlternativeAttribute(Reference ref);
};
