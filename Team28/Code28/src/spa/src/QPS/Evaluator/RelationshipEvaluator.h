#pragma once
#include "ClauseResult.h"
#include "../../PKB/QueryFacade.h"
#include "../../commons/Relationship/Relationship.h"

class RelationshipEvaluator {
private:
    QueryFacade* queryFacade;
public:
    explicit RelationshipEvaluator(QueryFacade* queryFacade) :
        queryFacade(queryFacade) {};
    static ClauseResult evaluate(Relationship<Reference, Reference> relationship);
};