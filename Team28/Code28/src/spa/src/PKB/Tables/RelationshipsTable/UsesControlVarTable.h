#pragma once

#include "StmtToVarRelationshipsTable.h"
#include <algorithm>

class UsesControlVarTable : public StmtToVarRelationshipsTable {
public:
    UsesControlVarTable();

    /*
     * Return list of possible values of all control statements.
     */
    std::vector<Value> getAllStmts();

    /*
     * Return list of possible values of control statements that
     * satisfies the given control variable varName.
     */
    std::vector<Value> getStmt(std::string varName);

    /*
     * Return list of possible (Statement, Variable) pairs which
     * are in the program.
     */
    std::vector<std::pair<Value, Value>> getStmtAndVar();

    std::vector<Value> getMatchingValue(std::string value, EntityName entity);
    std::map<Value, std::vector<Value>> getAllValues(EntityName entity);

private:
    std::vector<Value> allStmts;

    bool isProcessed = false;
};

class IfControlVarTable : public UsesControlVarTable {};
class WhileControlVarTable : public UsesControlVarTable {};
