#pragma once

#include "../Storage/Storage.h"

/*
 * A Facade class for QPS->PKB interactions.
 */
class QueryFacade {
public:
    /*
     * Explicit constructor for a QueryFacade.
     */
    explicit QueryFacade(Storage *storage);

    /*
     * Returns all statements inside the StatementsTable.
     */
    std::vector<Statement *> getAllStatements();

    /*
     * Returns all statements inside the StatementsTable.
     */
    std::vector<Statement *> getAllStatementsByType(StatementType type);

    /*
     * Returns all variable names inside the VariablesTable.
     */
    std::vector<std::string> getAllVariables();

    /*
     * Returns all constant names inside the ConstantsTable.
     */
    std::vector<std::string> getAllConstants();

    /*
     * Returns all procedure names inside the ProceduresTable.
     */
    std::vector<std::string> getAllProcedures();

    /*
     * Returns true if relationship holds between leftReference and
     * rightReference.
     */
    bool validate(RelationshipReference relType, Reference leftRef,
                  Reference rightRef);

    /*
     * Returns list of possible values that the right synonym can be based on
     * their relationship.
     */
    std::vector<Value> solveRight(RelationshipReference relType,
                                  Reference leftRef, EntityName rightSynonym);

    /*
     * Returns list of possible values that the left synonym can be based on
     * their relationship.
     */
    std::vector<Value> solveLeft(RelationshipReference relType,
                                 Reference rightRef, EntityName leftSynonym);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be
     * based on their relationship.
     */
    std::vector<std::pair<Value, Value>>
    solveBoth(RelationshipReference relType, EntityName leftSynonym,
              EntityName rightSynonym);

    /*
     * Return list of possible values of assignments that satisfy the given
     * varName and partial/wildcard expression.
     */
    std::vector<Value> getAssign(std::string varName, std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * varName and exact expression.
     */
    std::vector<Value> getAssignExact(std::string varName,
                                      std::string expression);

    /*
     * Return list of possible (Assignment, Variable) pairs which satisfy the
     * given partial/wildcard expression.
     */
    std::vector<std::pair<Value, Value>>
    getAssignAndVar(std::string expression);

    /*
     * Return list of possible (Assignment, Variable) pairs which satisfy the
     * given exact expression.
     */
    std::vector<std::pair<Value, Value>>
    getAssignAndVarExact(std::string expression);

    /*
     * Return list of possible values of Whiles that satisfy the given
     * varName and expression.
     */
    std::vector<Value> getWhile(std::string varName);

    /*
     * Return list of possible (While, Variable) pairs which satisfy the
     * given expression.
     */
    std::vector<std::pair<Value, Value>> getWhileAndVar();

    /*
     * Return list of possible values of Ifs that satisfy the given
     * varName and expression.
     */
    std::vector<Value> getIf(std::string varName);

    /*
     * Return list of possible (If, Variable) pairs which satisfy the
     * given expression.
     */
    std::vector<std::pair<Value, Value>> getIfAndVar();

    /*
     * Return attribute of the given stmtNum of a Print, Read or Call statement,
     * where attribute refers to the varName/procName used in the corresponding
     * statement number.
     */
    std::string getAttribute(int stmtNum);

private:
    Storage *storage;

    /*
     * Helper method for validating wildcard, checks against P and S tables.
     */
    bool validateWildcard(Reference leftRef, Reference rightRef,
                          Solvable *sTable, Solvable *pTable);

    /*
     * Helper method to concatenate solveRight results from a vector
     * of Solvable
     */
    std::vector<Value>
    concatSolveRightResults(std::vector<Solvable *> solvables,
                            Reference leftRef, EntityName rightSynonym);

    /*
     * Helper method to check RefType of given leftSynonym.
     */
    ReferenceType getRefType(EntityName leftSynonym);
};
