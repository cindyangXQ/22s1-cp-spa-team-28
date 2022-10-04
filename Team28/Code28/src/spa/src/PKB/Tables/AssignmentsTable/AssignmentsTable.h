#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

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
     * as well as expression -> (lineNo, variable)
     */
    void store(Assignment *assignment);

    int getTableSize() const;

    /*
     * Return boolean of whether the given varName and expression exists in an
     * assignment.
     */
    std::vector<Value> containsVarAndExpr(std::string varName,
                                          std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * varName and partial expression
     */
    std::vector<Value> getAssign(std::string varName, std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * varName and exact expression
     */
    std::vector<Value> getAssignExact(std::string varName,
                                      std::string expression);

    /*
     * Return list of possible (Assignment, Variable) pairs which satisfy the
     * given partial expression
     */
    std::vector<std::pair<Value, Value>>
    getAssignAndVar(std::string expression);

    /*
     * Return list of possible (Assignment, Variable) pairs which satisfy the
     * given exact expression
     */
    std::vector<std::pair<Value, Value>>
    getAssignAndVarExact(std::string expression);

private:
    int tableSize = 0;

    std::vector<Value> allLineNumbers;
    std::vector<Assignment> allAssignments;

    /*
     * Return list of possible values of assignments that satisfy the given
     * non-wildcarded varName and partial expression
     */
    std::vector<Value> getAssignFromVarAndExpr(std::string varName,
                                               std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * non-wildcarded varName and exact expression
     */
    std::vector<Value> getAssignFromVarAndExprExact(std::string varName,
                                                    std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * partial expression
     */
    std::vector<Value> getAssignFromExpr(std::string expression);

    /*
     * Return list of possible values of assignments that satisfy the given
     * exact expression
     */
    std::vector<Value> getAssignFromExprExact(std::string expression);
};
