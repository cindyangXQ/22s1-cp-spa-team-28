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
                  std::vector<QueryClause *> clauses,
                  std::vector<QueryClause *> withCls)
        : decl(decl), selectClause(selectClause), clauses(clauses),
          withCls(withCls){};

    Declaration getDecl();
    SelectClause getSelectClause();
    std::vector<QueryClause *> getQueryClause();
    std::vector<QueryClause *> getWithClause();
    std::vector<std::vector<QueryClause *>> getClauseGroup();
    void replaceWithClause(std::vector<QueryClause *> newWithClause);
    void setClauseGroup(std::vector<std::vector<QueryClause *>> *clauseGroups);

private:
    Declaration decl;
    SelectClause selectClause;
    std::vector<QueryClause *> clauses;
    std::vector<QueryClause *> withCls;
    std::vector<std::vector<QueryClause *>> clauseGroups;
};
