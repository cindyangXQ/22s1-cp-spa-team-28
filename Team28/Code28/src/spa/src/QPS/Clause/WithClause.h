#pragma once

#include "QueryClause.h"

/*
 * Class encapsulating the logic of the with clause.
 */
class WithClause : public QueryClause {
public:
    WithClause(){};
    Reference getRefLeft();
    Reference getRefRight();
    void parse(std::smatch matches, std::vector<Synonym> syns);
    bool validate();
    ClauseResult evaluate(QueryFacade *queryFacade);
    std::unordered_set<std::string> getSynonymsUsed();
    void populateOptimizeScore(QueryFacade *queryFacade);
    double getOptimizeScore();
    bool replace(Reference synRef, Reference valRef);

private:
    Reference refLeft;
    Reference refRight;
    std::unordered_set<std::string> synsUsed;
    double score;
    void populateSynsUsed();
    ClauseResult handleBothSynonyms(QueryFacade *queryFacade);
    ClauseResult handleOneSynonym(QueryFacade *queryFacade);
};
