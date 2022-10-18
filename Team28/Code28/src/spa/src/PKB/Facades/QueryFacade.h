#pragma once

#include "../../commons/AssignExpression.h"
#include "../../commons/StringUtil.h"
#include "../Storage/Storage.h"

/*
 * Encapsulates a Facade class for QPS->PKB interactions.
 */
class QueryFacade {
public:
    /*
     * Explicit constructor for a QueryFacade.
     */
    explicit QueryFacade(Storage *storage);

    /*
     * Returns all statements inside the source program that correspond to a
     * given StatementType.
     */
    std::vector<Statement *> getAllStatementsByType(StatementType type);

    /*
     * Returns all names inside the source program based on the designated
     * entity.
     */
    std::vector<std::string> getAllEntities(Designation entity);

    /*
     * Returns a boolean indicating if a provided relationship type holds
     * between the left reference and right reference.
     */
    bool validate(RelationshipReference relType, Reference leftRef,
                  Reference rightRef);

    /*
     * Returns all possible values that the right synonym can take on based on
     * the provided relationship type.
     */
    std::vector<Value> solveRight(RelationshipReference relType,
                                  Reference leftRef, EntityName rightSynonym);

    /*
     * Returns all possible values that the left synonym can take on based on
     * the provided relationship type.
     */
    std::vector<Value> solveLeft(RelationshipReference relType,
                                 Reference rightRef, EntityName leftSynonym);

    /*
     * Returns all possible pairs of values that the left and right synonyms can
     * take on based on the provided relationship type.
     */
    std::vector<std::pair<Value, Value>>
    solveBoth(RelationshipReference relType, EntityName leftSynonym,
              EntityName rightSynonym);

    /*
     * Returns list of possible values that the synonym can be based on the
     * given value and it's attribute.
     */
    std::vector<Value> solveOneAttribute(Reference ref, Value value);

    /*
     * Returns list of possible (Value, Value) that the pair of synonyms can be
     * based on their attributes.
     */
    std::vector<std::pair<Value, Value>> solveBothAttribute(Reference left,
                                                            Reference right);

    /*
     * Returns all possible values of assignments that satisfy the given varName
     * and partial/wildcard expression.
     */
    std::vector<Value> getAssign(std::string varName,
                                 AssignExpression expression);

    /*
     * Returns all possible pairs of assignments and variables that satisfy the
     * given expression.
     */
    std::vector<std::pair<Value, Value>>
    getAssignAndVar(AssignExpression expression);

    /*
     * Returns all possible values of conditional statements that satisfy
     * the given varName based on the designated conditional desType.
     * NOTE: desType == IF_C || desType == WHILE_C
     */
    std::vector<Value> getCond(Designation desType, std::string varName);

    /*
     * Returns all possible pairs of Cond and Variable based on the designated
     * conditional desType.
     * NOTE: desType == IF_C || desType == WHILE_C
     */
    std::vector<std::pair<Value, Value>> getCondAndVar(Designation desType);

    /*
     * Returns the secondary attribute of the given stmtNum of a Print, Read or
     * Call statement.
     */
    std::string getSecondaryAttribute(int stmtNum);

    std::vector<Value> getReflexiveNextT(EntityName stmtEntity);

private:
    Storage *storage;
    const std::string STMT_NO_SECONDARY_ATTRIBUTE =
        "StmtNum does not have a secondary attribute";

    /*
     * Helper method for validating wildcard, checks against P and S tables.
     */
    bool validateWildcard(Reference leftRef, Reference rightRef,
                          Solvable *sTable, Solvable *pTable);

    /*
     * Helper method to concatenate solveRight results from a vector
     * of Solvable.
     */
    std::vector<Value>
    concatSolveRightResults(std::vector<Solvable *> solvables,
                            Reference leftRef, EntityName rightSynonym);

    /*
     * Helper method to check RefType of given leftSynonym.
     */
    ReferenceType getRefType(EntityName leftSynonym);

    /*
     * Helper method to add all pairs of Values from cross intersection of left
     * and right.
     */
    void addAllPairsInto(std::vector<std::pair<Value, Value>> *result,
                         std::vector<Value> *left, std::vector<Value> *right);

    /*
     *Helper method to check if the query is a wildcarded uses *query.
     */
    bool isWildcardedUses(ReferenceType leftRef, RelationshipReference relType);

    /*
     * Helper method to check if the query is a wildcarded modifies
     * query.
     */
    bool isWildcardedModifies(ReferenceType leftRef,
                              RelationshipReference relType);
};
