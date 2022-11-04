#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "../../../commons/AssignExpression.h"
#include "../../../commons/Assignment.h"
#include "../../../commons/Value.h"
#include "../Table.h"

typedef std::pair<int, std::string> IntStringPair;

/*
 * Class encapsulating a Table used to store SIMPLE statements.
 */
class AssignmentsTable : public Table {
public:
    AssignmentsTable();

    /*
     * Stores an Assignment into the AssignmentsTable.
     * Mapping will be done from variable -> (lineNo, expression)
     * as well as expression -> (lineNo, variable).
     */
    void store(TableValue *assignment);

    int getTableSize();

    /*
     * Returns string form of all assignments.
     */
    std::vector<std::string> getAllAsString();

    /*
     * Return list of values of assignments of the given varName and expression
     * if they exists in an assignment.
     */
    std::vector<Value> containsVarAndExpr(std::string varName,
                                          std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * varName and partial expression.
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
     * given partial expression.
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
     * Checks if at a given stmt no, checks if varName = expr holds.
     * Handles both exact and partial match.
     */
    bool validate(int stmtNo, std::string varName, AssignExpression expr);

    /*
     * Returns the variable used at a given stmt no. Returns empty vector if no
     * match.
     */
    std::vector<Value> getVar(int stmtNo, AssignExpression expr);

    std::vector<Value> getMatchingValue(std::string value, EntityName entity);

    std::map<Value, std::vector<Value>> getAllValues(EntityName entity);

private:
    const std::string WILDCARD_SYMBOL = "_";
    const std::string EMPTY_STRING = "";
    std::vector<Value> allLineNumbers;
    std::vector<Assignment> allAssignments;
    std::map<int, Assignment *> lineAssignmentMap;
    int tableSize = 0;

    /*
     * Return list of possible values of assignments that satisfy the given
     * non-wildcarded varName and partial expression.
     */
    std::vector<Value> getAssignFromVarAndExpr(std::string varName,
                                               std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * non-wildcarded varName and exact expression.
     */
    std::vector<Value> getAssignFromVarAndExprExact(std::string varName,
                                                    std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * partial expression.
     */
    std::vector<Value> getAssignFromExpr(std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * exact expression.
     */
    std::vector<Value> getAssignFromExprExact(std::string expression);
};
