#include "AffectsTTable.h"

void AffectsTTable::initAffectsT(StorageView *storage) {
    AffectsTable *affects = storage->getTable<AffectsTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    this->matrix = affects->eagerGetMatrix();
    this->assignments =
        statements->getStatementsSetByType(StatementType::ASSIGN);
}

void AffectsTTable::populateAffectsT() { this->matrix = computeClosure(); }

std::map<std::pair<int, int>, bool> AffectsTTable::computeClosure() {
    if (isComputed) {
        return this->matrix;
    }
    // TODO: Clean up/change method entirely
    std::vector<std::pair<int, int>> validPos;
    std::map<std::pair<int, int>, bool> intermediate;
    std::map<std::pair<int, int>, bool> final;

    for (int i : this->assignments) {
        for (int j : this->assignments) {
            std::pair<int, int> curr = std::make_pair(i, j);
            intermediate[curr] = false;
        }
    }
    for (const auto &elem : this->matrix) {
        intermediate[elem.first] = elem.second;
        validPos.push_back(elem.first);
    }
    for (int k : this->assignments) {
        for (int i : this->assignments) {
            for (int j : this->assignments) {
                std::pair<int, int> curr = std::make_pair(i, j);
                std::pair<int, int> left = std::make_pair(i, k);
                std::pair<int, int> right = std::make_pair(k, j);
                intermediate[curr] =
                    intermediate[curr] ||
                    (intermediate[left] && intermediate[right]);
            }
        }
    }
    for (std::pair pair : validPos) {
        final[pair] = intermediate[pair];
    }
    this->isComputed = true;
    this->matrix = final;
    return this->matrix;
}

bool AffectsTTable::validate(Reference leftRef, Reference rightRef) {
    if (!isComputed) {
        this->computeClosure();
    }
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                std::pair curr = std::make_pair(left, right);
                if (this->matrix[curr]) {
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
            std::pair curr = std::make_pair(left, right);
            if (this->matrix[curr]) {
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
            std::pair curr = std::make_pair(left, right);
            if (this->matrix[curr]) {
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
    std::pair curr = std::make_pair(left, right);
    return this->matrix[curr];
};

std::vector<Value> AffectsTTable::solveRight(Reference leftRef,
                                             EntityName rightSynonym,
                                             StorageView *storage) {
    if (!isAssignmentEntity(rightSynonym)) {
        return std::vector<Value>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    std::unordered_set<Value> intermediateResult;
    if (leftRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                std::pair curr = std::make_pair(left, right);
                if (this->matrix[curr]) {
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
            std::pair curr = std::make_pair(left, right);
            if (this->matrix[curr]) {
                intermediateResult.insert(
                    Value(ValueType::STMT_NUM, toString(right)));
            }
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    return result;
}

std::vector<Value> AffectsTTable::solveLeft(Reference rightRef,
                                            EntityName leftSynonym,
                                            StorageView *storage) {
    if (!isAssignmentEntity(leftSynonym)) {
        return std::vector<Value>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    std::unordered_set<Value> intermediateResult;
    if (rightRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                std::pair curr = std::make_pair(left, right);
                if (this->matrix[curr]) {
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
            std::pair curr = std::make_pair(left, right);
            if (this->matrix[curr]) {
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
AffectsTTable::solveBoth(EntityName leftSynonym, EntityName rightSynonym,
                         StorageView *storage) {
    if (!isAssignmentEntity(leftSynonym) || !isAssignmentEntity(rightSynonym)) {
        return std::vector<std::pair<Value, Value>>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    std::vector<std::pair<Value, Value>> result;
    for (int left : this->assignments) {
        for (int right : this->assignments) {
            std::pair curr = std::make_pair(left, right);
            if (this->matrix[curr]) {
                Value leftValue = Value(ValueType::STMT_NUM, toString(left));
                Value rightValue = Value(ValueType::STMT_NUM, toString(right));
                result.push_back(std::make_pair(leftValue, rightValue));
            }
        }
    }
    return result;
}

std::vector<Value> AffectsTTable::solveBothReflexive(EntityName synonym,
                                                     StorageView *storage) {
    if (!isAssignmentEntity(synonym)) {
        return std::vector<Value>();
    }
    if (!isComputed) {
        this->computeClosure();
    }
    std::vector<Value> result;
    for (int stmt : this->assignments) {
        std::pair curr = std::make_pair(stmt, stmt);
        if (this->matrix[curr]) {
            Value stmtValue = Value(ValueType::STMT_NUM, toString(stmt));
            result.push_back(stmtValue);
        }
    }
    return result;
}

bool AffectsTTable::isAssignment(int stmt) {
    return this->assignments.count(stmt) > 0;
};

bool AffectsTTable::areAssignments(int left, int right) {
    return (this->assignments.count(left) > 0) &&
           (this->assignments.count(right) > 0);
};

bool AffectsTTable::isAssignmentEntity(EntityName entity) {
    return (entity == EntityName::ASSIGN) || (entity == EntityName::STMT);
};
