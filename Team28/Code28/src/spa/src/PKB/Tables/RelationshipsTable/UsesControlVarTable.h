#pragma once

#include "StmtToVarRelationshipsTable.h"
#include <algorithm>

class UsesControlVarTable : public StmtToVarRelationshipsTable {
public:
    /*
     * Constructor for UsesControlVarTable.
     */
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

    /*
     * Checks if at a given stmt no, varName is used in the conditional.
     */
    using StmtToVarRelationshipsTable::validate;
    bool validate(int stmtNo, std::string varName);

    /*
     * Returns all variables used in the conditional at a given stmt no.
     */
    std::vector<Value> getVar(int stmtNo);

    std::vector<Value> getMatchingValue(std::string value, EntityName entity,
                                        StorageView *storage);
    std::map<Value, std::vector<Value>> getAllValues(EntityName entity,
                                                     StorageView *storage);

private:
    const std::string WILDCARD_SYMBOL = "_";
    std::vector<Value> allStmts;

    bool isProcessed = false;
};

class IfControlVarTable : public UsesControlVarTable {};
class WhileControlVarTable : public UsesControlVarTable {};
