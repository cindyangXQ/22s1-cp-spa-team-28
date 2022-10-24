#pragma once

#include "../Clause/Declaration.h"
#include "../Clause/QueryClause.h"
#include "../Clause/SelectClause.h"
#include <iostream>

/*
 * Class encapsulating the logical representation of the query after parsing.
 */
class SolvableQuery {
public:
    ~SolvableQuery();
    SolvableQuery(Declaration decl, SelectClause selectClause,
                  std::vector<QueryClause *> clauses)
        : decl(decl), selectClause(selectClause), clauses(clauses){};

    Declaration getDecl();
    SelectClause getSelectClause();
    std::vector<QueryClause *> getQueryClause();

private:
    Declaration decl;
    SelectClause selectClause;
    std::vector<QueryClause *> clauses;
};
