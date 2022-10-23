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

private:
    Reference refLeft;
    Reference refRight;
    ClauseResult handleBothSynonyms(QueryFacade *queryFacade);
    ClauseResult handleOneSynonym(QueryFacade *queryFacade);
};
