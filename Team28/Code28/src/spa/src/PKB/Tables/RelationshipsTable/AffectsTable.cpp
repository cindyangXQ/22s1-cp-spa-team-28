#include "AffectsTable.h"

void AffectsTable::initAffects(StorageView *storage) {
    this->next = storage->getTable<NextTable>();
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
    std::unordered_set<Value> intermediateResult;
    if (leftRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                if (checkAffects(left, right)) {
                    intermediateResult.insert(
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
                intermediateResult.insert(
                    Value(ValueType::STMT_NUM, toString(right)));
            }
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    return result;
}

std::vector<Value> AffectsTable::solveLeft(Reference rightRef,
                                           EntityName leftSynonym,
                                           StorageView *storage) {
    if (!isAssignmentEntity(leftSynonym)) {
        return std::vector<Value>();
    }
    std::unordered_set<Value> intermediateResult;
    if (rightRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                if (checkAffects(left, right)) {
                    intermediateResult.insert(
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
                intermediateResult.insert(
                    Value(ValueType::STMT_NUM, toString(left)));
            }
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
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

bool AffectsTable::isAffects(int s2, std::string v) {
    return isAssignment(s2) && this->usesS->isRelationshipExist(s2, v);
};

bool AffectsTable::isAssignment(int stmt) {
    return this->assignments.count(stmt) > 0;
};

bool AffectsTable::areAssignments(int left, int right) {
    return (this->assignments.count(left) > 0) &&
           (this->assignments.count(right) > 0);
};

bool AffectsTable::isModified(int stmt, std::string v) {
    return isModifiableStmt(stmt) &&
           this->modifiesS->isRelationshipExist(stmt, v);
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
        calculateAffects(left);
    }

    return this->matrix[curr] == Status::TRUE;
};

void AffectsTable::calculateAffects(int stmt) {
    // assumption: an assign stmt only modifies 1 variable
    std::string modifiedVariable = this->modifiesS->retrieveSingleRight(stmt);

    std::map<int, int> visited;
    visited[stmt] = 1;
    for (int i : next->retrieveLeft(stmt)) {
        calculateAffectsHelper(stmt, i, modifiedVariable, visited);
    }
    for (int assign : assignments) {
        std::pair<int, int> pair = std::make_pair(stmt, assign);
        if (this->matrix[pair] != Status::TRUE) {
            this->matrix[pair] = Status::FALSE;
        }
    }
};

void AffectsTable::calculateAffectsHelper(int start, int current,
                                          std::string modifiedVariable,
                                          std::map<int, int> visited) {
    if (isAffects(current, modifiedVariable)) {
        this->matrix[std::make_pair(start, current)] = Status::TRUE;
    }

    visited[current] += 1;
    if (visited[current] > 2) {
        return;
    }

    if (isModified(current, modifiedVariable)) {
        return;
    }

    for (int i : next->retrieveLeft(current)) {
        calculateAffectsHelper(start, i, modifiedVariable, visited);
    }
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

std::map<std::pair<int, int>, bool> AffectsTable::eagerGetMatrix() {
    std::map<std::pair<int, int>, bool> result = {};
    for (const auto &p : this->matrix) {
        if (p.second == Status::UNKNOWN) {
            std::pair curr = p.first;
            calculateAffects(curr.first);
        }
    }
    for (const auto &p : this->matrix) {
        bool status = p.second == Status::TRUE ? true : false;
        std::pair curr = p.first;
        result[curr] = status;
    }
    return result;
}
