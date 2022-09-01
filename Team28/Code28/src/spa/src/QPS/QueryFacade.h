#pragma once

#include <tuple>
#include "SolvableQuery.h"
#include "Reference/Reference.h"
#include "Reference/StatementNumber.h"
#include "Reference/VariableName.h"

class QueryFacade {
public:
    static bool getRelationship(RelationshipReference relRef, StatementNumber stmtNum, VariableName varName);
    static std::vector<StatementNumber> getRelationshipByVariable(RelationshipReference relRef, VariableName varName);
    static std::vector<VariableName> getRelationshipByStatement(RelationshipReference relRef, StatementNumber stmtNum);
    static std::vector<StatementNumber, VariableName> getAllRelationship(RelationshipReference relRef);
    static std::vector<VariableName> getAllVariable();
    static std::vector<StatementNumber> getAllStatement();
};