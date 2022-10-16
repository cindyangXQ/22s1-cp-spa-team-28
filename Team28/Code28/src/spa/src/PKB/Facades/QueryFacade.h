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
     * Returns all statements inside the StatementsTable based on StatementType.
     */
    std::vector<Statement *> getAllStatementsByType(StatementType type);

    /*
     * Returns all entity names from the given Entity.
     */
    std::vector<std::string> getAllEntities(Designation entity);

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
     * Return list of possible values of conditional statements that satisfy the
     * given varName based on the conditional stated in designation.
     */
    std::vector<Value> getCond(Designation desType, std::string varName);

    /*
     * Return list of possible (Cond, Variable) pairs based on the conditional
     * stated in designation.
     */
    std::vector<std::pair<Value, Value>> getCondAndVar(Designation desType);

    /*
     * Return secondary attribute of the given stmtNum of a Print, Read or Call
     * statement.
     */
    std::string getSecondaryAttribute(int stmtNum);

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
