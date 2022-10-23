#include "AffectsTable.h"

void AffectsTable::initAffects(StorageView *storage) {
    this->next = storage->getTable<NextTable>();
    this->nextT = storage->getTable<NextTTable>();
    this->modifiesS = storage->getTable<ModifiesSTable>();
    this->usesS = storage->getTable<UsesSTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    this->assignments =
        statements->getStatementsSetByType(StatementType::ASSIGN);
    std::unordered_set<int> calls =
        statements->getStatementsSetByType(StatementType::CALL);
    this->modifiableStatements =
        statements->getStatementsSetByType(StatementType::READ);
    this->modifiableStatements.insert(calls.begin(), calls.end());
    this->modifiableStatements.insert(this->assignments.begin(),
                                      this->assignments.end());
    this->totalLines = statements->getTableSize();

    for (int i : this->assignments) {
        for (int j : this->assignments) {
            std::pair<int, int> curr = std::make_pair(i, j);
            this->matrix[curr] = Status::UNKNOWN;
        }
    }
}

bool AffectsTable::validate(Reference leftRef, Reference rightRef) {
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                if (checkAffects(left, right)) {
                    return true;
                }
            }
        }
        return false;
    }
    if (leftRef.isWildcard()) {
        int right = convertToType<int>(rightRef.getValueString());
        if (!isAssignment(right)) {
            return false;
        }
        for (int left : this->assignments) {
            if (checkAffects(left, right)) {
                return true;
            }
        }
        return false;
    }
    if (rightRef.isWildcard()) {
        int left = convertToType<int>(leftRef.getValueString());
        if (!isAssignment(left)) {
            return false;
        }
        for (int right : this->assignments) {
            if (checkAffects(left, right)) {
                return true;
            }
        }
        return false;
    }
    int left = convertToType<int>(leftRef.getValueString());
    int right = convertToType<int>(rightRef.getValueString());
    if (!areAssignments(left, right)) {
        return false;
    }
    return checkAffects(left, right);
};

std::vector<Value> AffectsTable::solveRight(Reference leftRef,
                                            EntityName rightSynonym,
                                            StorageView *storage) {
    if (!isAssignmentEntity(rightSynonym)) {
        return std::vector<Value>();
    }
    std::vector<Value> result;
    if (leftRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                if (checkAffects(left, right)) {
                    result.push_back(
                        Value(ValueType::STMT_NUM, toString(right)));
                }
            }
        }
    } else {
        int left = convertToType<int>(leftRef.getValueString());
        if (!isAssignment(left)) {
            return std::vector<Value>();
        }
        for (int right : this->assignments) {
            if (checkAffects(left, right)) {
                result.push_back(Value(ValueType::STMT_NUM, toString(right)));
            }
        }
    }

    return result;
}

std::vector<Value> AffectsTable::solveLeft(Reference rightRef,
                                           EntityName leftSynonym,
                                           StorageView *storage) {
    if (!isAssignmentEntity(leftSynonym)) {
        return std::vector<Value>();
    }
    std::vector<Value> result;
    if (rightRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                if (checkAffects(left, right)) {
                    result.push_back(
                        Value(ValueType::STMT_NUM, toString(left)));
                }
            }
        }
    } else {
        int right = convertToType<int>(rightRef.getValueString());
        if (!isAssignment(right)) {
            return std::vector<Value>();
        }
        for (int left : this->assignments) {
            if (checkAffects(left, right)) {
                result.push_back(Value(ValueType::STMT_NUM, toString(left)));
            }
        }
    }
    return result;
};

std::vector<std::pair<Value, Value>>
AffectsTable::solveBoth(EntityName leftSynonym, EntityName rightSynonym,
                        StorageView *storage) {
    if (!isAssignmentEntity(leftSynonym) || !isAssignmentEntity(rightSynonym)) {
        return std::vector<std::pair<Value, Value>>();
    }
    std::vector<std::pair<Value, Value>> result;
    for (int left : this->assignments) {
        for (int right : this->assignments) {
            if (checkAffects(left, right)) {
                Value leftValue = Value(ValueType::STMT_NUM, toString(left));
                Value rightValue = Value(ValueType::STMT_NUM, toString(right));
                result.push_back(std::make_pair(leftValue, rightValue));
            }
        }
    }
    return result;
}

std::vector<Value> AffectsTable::solveBothReflexive(EntityName synonym,
                                                    StorageView *storage) {
    if (!isAssignmentEntity(synonym)) {
        return std::vector<Value>();
    }
    std::vector<Value> result;
    for (int stmt : this->assignments) {
        if (checkAffects(stmt, stmt)) {
            Value stmtValue = Value(ValueType::STMT_NUM, toString(stmt));
            result.push_back(stmtValue);
        }
    }
    return result;
}

bool AffectsTable::isAssignment(int stmt) {
    return this->assignments.count(stmt) > 0;
};

bool AffectsTable::areAssignments(int left, int right) {
    return (this->assignments.count(left) > 0) &&
           (this->assignments.count(right) > 0);
};

bool AffectsTable::isModifiableStmt(int stmt) {
    return this->modifiableStatements.count(stmt) > 0;
};

bool AffectsTable::isAssignmentEntity(EntityName entity) {
    return (entity == EntityName::ASSIGN) || (entity == EntityName::STMT);
};

bool AffectsTable::checkAffects(int left, int right) {
    std::pair<int, int> curr = std::make_pair(left, right);
    if (this->matrix[curr] == Status::UNKNOWN) {
        calculateAffects(left, right);
    }

    return this->matrix[curr] == Status::TRUE;
};

void AffectsTable::calculateAffects(int left, int right) {
    if (nextT->retrieveLeft(left).count(right) == 0) {
        // no path from left to right
        this->matrix[std::make_pair(left, right)] = Status::FALSE;
        return;
    }
    std::vector<std::string> commonVariables = getCommonVariables(left, right);
    if (commonVariables.size() == 0) {
        // no common variable
        this->matrix[std::make_pair(left, right)] = Status::FALSE;
        return;
    }
    std::map<int, int> visited;
    visited[left] = 1;
    for (int i : next->retrieveLeft(left)) {
        if (i == right || nextT->retrieveLeft(i).count(right) > 0) {
            if (calculateAffectsHelper(i, right, commonVariables, visited)) {
                this->matrix[std::make_pair(left, right)] = Status::TRUE;
                return;
            } else {
                this->matrix[std::make_pair(left, right)] = Status::FALSE;
            }
        }
    }
};

bool AffectsTable::calculateAffectsHelper(
    int current, int goal, std::vector<std::string> commonVariables,
    std::map<int, int> visited) {
    if (current == goal) {
        return true;
    }
    visited[current] += 1;
    if (visited[current] > 2) {
        return false;
    }
    std::vector<std::string> remainingVariables;
    if (isModifiableStmt(current)) {
        remainingVariables = getRemainingVariables(&commonVariables, current);
        if (remainingVariables.size() == 0) {
            return false;
        }
    } else {
        remainingVariables = commonVariables;
    }

    for (int i : next->retrieveLeft(current)) {
        if (nextT->retrieveLeft(i).count(goal) > 0 || i == goal) {
            if (calculateAffectsHelper(i, goal, commonVariables, visited)) {
                return true;
            }
        }
    }

    return false;
};

std::vector<std::string> AffectsTable::getCommonVariables(int left, int right) {
    std::unordered_set<std::string> modifiedV =
        this->modifiesS->retrieveLeft(left);
    std::unordered_set<std::string> usedV = this->usesS->retrieveLeft(right);
    std::vector<std::string> commonV;
    for (std::string variable : modifiedV) {
        if (usedV.count(variable) > 0) {
            commonV.push_back(variable);
        }
    }
    return commonV;
};

std::vector<std::string>
AffectsTable::getRemainingVariables(std::vector<std::string> *variables,
                                    int stmt) {
    std::unordered_set<std::string> modifiedV =
        this->modifiesS->retrieveLeft(stmt);
    std::vector<std::string> remainingV;
    for (std::string variable : *variables) {
        if (modifiedV.count(variable) == 0) {
            remainingV.push_back(variable);
        }
    }
    return remainingV;
};
