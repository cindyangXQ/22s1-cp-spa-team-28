#include "AssignmentsTable.h"

AssignmentsTable::AssignmentsTable() = default;

void AssignmentsTable::store(Assignment *assignment) {
    int lineNo = assignment->getLineNo();
    this->allAssignments.push_back(*assignment);
    this->allLineNumbers.push_back(
        Value(ValueType::STMT_NUM, std::to_string(lineNo)));
};

std::vector<Value>
AssignmentsTable::containsVarAndExpr(std::string varName,
                                     std::string expression) {
    std::unordered_set<Value> intermediateResult;

    for (Assignment assignment : this->allAssignments) {
        if (assignment.getVariable() == varName &&
            assignment.getExpression() == expression) {
            intermediateResult.insert(Value(
                ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<Value>
AssignmentsTable::getAssignFromVarAndExpr(std::string varName,
                                          std::string expression) {
    if (expression == "_") {
        expression = "";
    }

    std::unordered_set<Value> intermediateResult;

    for (Assignment assignment : this->allAssignments) {
        if (assignment.getVariable() != varName) {
            continue;
        }
        if (assignment.getExpression().find(expression) != std::string::npos) {
            intermediateResult.insert(Value(
                ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<Value>
AssignmentsTable::getAssignFromVarAndExprExact(std::string varName,
                                               std::string expression) {
    std::unordered_set<Value> intermediateResult;

    for (Assignment assignment : this->allAssignments) {
        if (assignment.getVariable() != varName) {
            continue;
        }
        if (assignment.getExpression() == expression) {
            intermediateResult.insert(Value(
                ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<Value> AssignmentsTable::getAssignFromExpr(std::string expression) {
    std::unordered_set<Value> intermediateResult;

    for (Assignment assignment : this->allAssignments) {
        if (assignment.getExpression().find(expression) != std::string::npos) {
            intermediateResult.insert(Value(
                ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<Value>
AssignmentsTable::getAssignFromExprExact(std::string expression) {
    std::unordered_set<Value> intermediateResult;

    for (Assignment assignment : this->allAssignments) {
        if (assignment.getExpression() == expression) {
            intermediateResult.insert(Value(
                ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<Value> AssignmentsTable::getAssign(std::string varName,
                                               std::string expression) {
    if (varName == "_" && expression == "_") {
        return this->allLineNumbers;
    }
    if (varName == "_") {
        return this->getAssignFromExpr(expression);
    }
    return this->getAssignFromVarAndExpr(varName, expression);
};

std::vector<Value> AssignmentsTable::getAssignExact(std::string varName,
                                                    std::string expression) {
    if (varName == "_") {
        return this->getAssignFromExprExact(expression);
    }
    return this->getAssignFromVarAndExprExact(varName, expression);
};

std::vector<std::pair<Value, Value>>
AssignmentsTable::getAssignAndVar(std::string expression) {
    if (expression == "_") {
        expression = "";
    }

    std::unordered_set<std::pair<Value, Value>, value_pair_hash>
        intermediateResult;

    for (Assignment assignment : this->allAssignments) {
        if (assignment.getExpression().find(expression) != std::string::npos) {
            intermediateResult.insert(std::make_pair(
                Value(ValueType::STMT_NUM,
                      std::to_string(assignment.getLineNo())),
                Value(ValueType::VAR_NAME, assignment.getVariable())));
        }
    }
    std::vector<std::pair<Value, Value>> result =
        std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                             intermediateResult.end());
    std::sort(result.begin(), result.end(), value_pair_sort());
    return result;
};

std::vector<std::pair<Value, Value>>
AssignmentsTable::getAssignAndVarExact(std::string expression) {
    std::unordered_set<std::pair<Value, Value>, value_pair_hash>
        intermediateResult;

    for (Assignment assignment : this->allAssignments) {
        if (assignment.getExpression() == expression) {
            intermediateResult.insert(std::make_pair(
                Value(ValueType::STMT_NUM,
                      std::to_string(assignment.getLineNo())),
                Value(ValueType::VAR_NAME, assignment.getVariable())));
        }
    }
    std::vector<std::pair<Value, Value>> result =
        std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                             intermediateResult.end());
    std::sort(result.begin(), result.end(), value_pair_sort());
    return result;
};
