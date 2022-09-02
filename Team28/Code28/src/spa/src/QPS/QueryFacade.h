#pragma once

#include <tuple>
#include "SolvableQuery.h"
#include "Reference/Reference.h"
#include "Reference/StatementNumber.h"
#include "Reference/VariableName.h"

/*
 * Facade class responsible for communicating with PKB.
 */
class QueryFacade {
public:
    static bool getRelationship(RelRef relRef, StatementNumber stmtNum, VariableName varName);
    static std::vector<StatementNumber> getRelationshipByVariable(RelRef relRef, VariableName varName);
    static std::vector<VariableName> getRelationshipByStatement(RelRef relRef, StatementNumber stmtNum);
    static std::vector<StatementNumber, VariableName> getAllRelationship(RelRef relRef);
    static std::vector<VariableName> getAllVariable();
    static std::vector<StatementNumber> getAllStatement();
};