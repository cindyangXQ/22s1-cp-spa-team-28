#pragma once

#include "../../SP/SP.h"
#include "../Utils.h"
#include "QueryClause.h"

const std::unordered_set<EntityName> PATTERN_ENTITY_MAP = {
    EntityName::ASSIGN, EntityName::IF, EntityName::WHILE};
typedef std::string Expression;

/*
 * Class encapsulating the logic of the pattern clause.
 */
class PatternClause : public QueryClause {
public:
    PatternClause(){};
    Reference getStmtRef();
    Reference getEntRef();
    Expression getExpression();
    bool getIsExact();
    void parse(std::smatch matches, std::vector<Synonym> syns);
    bool validate();
    ClauseResult evaluate(QueryFacade *queryFacade);
    std::unordered_set<std::string> getSynonymsUsed();
    double getOptimizeScore();
    bool replace(Reference synRef, Reference valRef);

private:
    EntityName patternType;
    Reference stmtRef;
    Reference entRef;
    Expression expression;
    bool isExact;
    std::unordered_set<std::string> synsUsed;
    void populateSynsUsed();
    ClauseResult handleNoSynonym(QueryFacade *queryFacade);
    ClauseResult handleLeftSynonym(QueryFacade *queryFacade);
    ClauseResult handleRightSynonym(QueryFacade *queryFacade);
    ClauseResult handleBothSynonym(QueryFacade *queryFacade);
};
