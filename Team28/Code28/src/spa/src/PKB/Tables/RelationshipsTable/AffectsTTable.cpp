#include "AffectsTTable.h"

void AffectsTTable::initAffectsT(StorageView *storage) {
    this->affects = storage->getTable<AffectsTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    this->assignments =
        statements->getStatementsSetByType(StatementType::ASSIGN);
}

void AffectsTTable::resetCache() {
    if (!shouldTableReset()) {
        return;
    }
    this->matrix.clear();
    this->isComputed = false;
    markTableResetted();
}

std::map<std::pair<int, int>, bool> AffectsTTable::computeClosure() {
    if (isComputed) {
        return this->matrix;
    }
    for (int i : this->assignments) {
        for (int j : this->assignments) {
            std::pair<int, int> curr = std::make_pair(i, j);
            this->matrix[curr] = false;
        }
    }
    for (const auto &elem : this->affects->eagerGetMatrix()) {
        this->matrix[elem.first] = elem.second;
    }
    for (int k : this->assignments) {
        for (int i : this->assignments) {
            for (int j : this->assignments) {
                std::pair<int, int> curr = std::make_pair(i, j);
                std::pair<int, int> left = std::make_pair(i, k);
                std::pair<int, int> right = std::make_pair(k, j);
                matrix[curr] = matrix[curr] || (matrix[left] && matrix[right]);
            }
        }
    }
    this->isComputed = true;
    markForReset();
    return this->matrix;
}

bool AffectsTTable::validate(Reference leftRef, Reference rightRef) {
    if (!isComputed) {
        this->computeClosure();
    }
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        return verifyDoubleWildcards();
    }
    if (leftRef.isWildcard()) {
        int right = convertToType<int>(rightRef.getValueString());
        return verifyLeftWildcard(right);
    }
    if (rightRef.isWildcard()) {
        int left = convertToType<int>(leftRef.getValueString());
        return verifyRightWildcard(left);
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
        solveSingleWildcard(&intermediateResult, Position::RIGHT);
    } else {
        int left = convertToType<int>(leftRef.getValueString());
        if (!isAssignment(left)) {
            return std::vector<Value>();
        }
        solveHelper(left, &intermediateResult, Position::LEFT);
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
        solveSingleWildcard(&intermediateResult, Position::LEFT);
    } else {
        int right = convertToType<int>(rightRef.getValueString());
        if (!isAssignment(right)) {
            return std::vector<Value>();
        }
        solveHelper(right, &intermediateResult, Position::RIGHT);
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

bool AffectsTTable::verifyDoubleWildcards() {
    for (int left : this->assignments) {
        for (int right : this->assignments) {
            std::pair curr = std::make_pair(left, right);
            if (this->matrix[curr]) {
                return true;
            }
        }
    }
    return false;
};

bool AffectsTTable::verifyLeftWildcard(int right) {
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

bool AffectsTTable::verifyRightWildcard(int left) {
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

void AffectsTTable::solveSingleWildcard(
    std::unordered_set<Value> *intermediateResult, Position stmtPos) {
    for (int left : this->assignments) {
        for (int right : this->assignments) {
            std::pair curr = std::make_pair(left, right);
            if (this->matrix[curr]) {
                intermediateResult->insert(
                    Value(ValueType::STMT_NUM,
                          toString(chooseStmt(left, right, stmtPos))));
            }
        }
    }
};

void AffectsTTable::solveHelper(int stmt,
                                std::unordered_set<Value> *intermediateResult,
                                Position stmtPos) {
    auto check = [&](const int &other) -> bool {
        std::pair<int, int> curr = stmtPos == Position::LEFT
                                       ? std::make_pair(stmt, other)
                                       : std::make_pair(other, stmt);
        return this->matrix[curr];
    };
    for (int other : this->assignments) {
        if (check(other)) {
            intermediateResult->insert(
                Value(ValueType::STMT_NUM, toString(other)));
        }
    }
};