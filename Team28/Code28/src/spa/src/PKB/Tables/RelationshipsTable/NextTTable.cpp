#include "NextTTable.h"

int NextTTable::getTableSize() { return INT_MAX; };

void NextTTable::initNextT(StorageView *storage) {
    this->next = storage->getTable<NextTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    this->totalLines = statements->getTableSize();
    initMatrix();
};

void NextTTable::initMatrix() {
    for (int i = 1; i <= this->totalLines; i++) {
        for (int j = 1; j <= this->totalLines; j++) {
            std::pair<int, int> curr = std::make_pair(i, j);
            this->matrix[curr] = false;
        }
    }
}

void NextTTable::resetCache() {
    if (!shouldTableReset()) {
        return;
    }
    this->isDFSComputed.clear();
    initMatrix();
    markTableResetted();
}

bool NextTTable::validate(Reference leftRef, Reference rightRef) {
    markForReset();
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

    return checkNextT(left, right);
};

std::vector<Value> NextTTable::solveRight(Reference leftRef,
                                          EntityName rightSynonym,
                                          StorageView *storage) {
    if (stmtRefSet.count(rightSynonym) == 0) {
        return std::vector<Value>();
    }
    markForReset();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleRights =
        getStatementsHelper(statements, rightSynonym);

    return solveRightHelper(&possibleRights, leftRef);
}

std::vector<Value> NextTTable::solveLeft(Reference rightRef,
                                         EntityName leftSynonym,
                                         StorageView *storage) {
    if (stmtRefSet.count(leftSynonym) == 0) {
        return std::vector<Value>();
    }
    markForReset();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleLefts =
        getStatementsHelper(statements, leftSynonym);

    return solveLeftHelper(&possibleLefts, rightRef);
};

std::vector<std::pair<Value, Value>>
NextTTable::solveBoth(EntityName leftSynonym, EntityName rightSynonym,
                      StorageView *storage) {
    if (stmtRefSet.count(leftSynonym) == 0 ||
        stmtRefSet.count(rightSynonym) == 0) {
        return std::vector<std::pair<Value, Value>>();
    }
    markForReset();
    StatementType leftType = Statement::getStmtTypeFromEntityName(leftSynonym);
    StatementType rightType =
        Statement::getStmtTypeFromEntityName(rightSynonym);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleLefts = statements->getStatementsByType(leftType);
    std::vector<int> possibleRights =
        statements->getStatementsByType(rightType);

    std::vector<std::pair<Value, Value>> result;
    for (int left : possibleLefts) {
        for (int right : possibleRights) {
            if (checkNextT(left, right)) {
                Value leftValue = Value(ValueType::STMT_NUM, toString(left));
                Value rightValue = Value(ValueType::STMT_NUM, toString(right));
                result.push_back(std::make_pair(leftValue, rightValue));
            }
        }
    }
    return result;
}

std::vector<Value> NextTTable::solveBothReflexive(EntityName stmtEntity,
                                                  StorageView *storage) {
    if (stmtRefSet.count(stmtEntity) == 0) {
        return std::vector<Value>();
    }
    markForReset();
    StatementType stmtType = Statement::getStmtTypeFromEntityName(stmtEntity);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleValues = statements->getStatementsByType(stmtType);

    std::vector<Value> result = {};

    for (int i : possibleValues) {
        if (checkNextT(i, i)) {
            result.push_back(Value(ValueType::STMT_NUM, toString(i)));
        }
    }
    return result;
}

bool NextTTable::checkNextT(int left, int right) {
    std::pair<int, int> curr = std::make_pair(left, right);
    if (isDFSComputed[left]) {
        return this->matrix[curr];
    }

    for (int i : this->next->retrieveLeft(left)) {
        std::pair<int, int> nextPair = std::make_pair(left, i);
        if (this->matrix[nextPair] == false) {
            DFSUtil(left, i);
        }
    }
    isDFSComputed[left] = true;
    return this->matrix[curr];
};

bool NextTTable::verifyDoubleWildcards() {
    for (int i = 1; i <= this->totalLines; i++) {
        for (int j = 1; j <= this->totalLines; j++) {
            if (checkNextT(i, j)) {
                return true;
            }
        }
    }
    return false;
};

bool NextTTable::verifyLeftWildcard(int right) {
    for (int i = 1; i <= this->totalLines; i++) {
        if (checkNextT(i, right)) {
            return true;
        }
    }
    return false;
};

bool NextTTable::verifyRightWildcard(int left) {
    for (int i = 1; i <= this->totalLines; i++) {
        if (checkNextT(left, i)) {
            return true;
        }
    }
    return false;
};

void NextTTable::DFSUtil(int s, int v) {
    std::pair<int, int> curr = std::make_pair(s, v);
    this->matrix[curr] = true;

    for (int i : this->next->retrieveLeft(v)) {
        std::pair<int, int> nextPair = std::make_pair(s, i);
        if (this->matrix[nextPair] == false) {
            if (s == i) {
                this->matrix[nextPair] = true;
            } else {
                DFSUtil(s, i);
            }
        }
    }
};

std::vector<Value>
NextTTable::solveRightHelper(std::vector<int> *possibleRights,
                             Reference leftRef) {
    std::unordered_set<Value> intermediateResult;
    if (leftRef.isWildcard()) {
        for (int right : *possibleRights) {
            if (verifyLeftWildcard(right)) {
                intermediateResult.insert(
                    Value(ValueType::STMT_NUM, toString(right)));
            }
        }
    } else {
        int left = convertToType<int>(leftRef.getValueString());
        for (int right : *possibleRights) {
            if (checkNextT(left, right)) {
                intermediateResult.insert(
                    Value(ValueType::STMT_NUM, toString(right)));
            }
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    return result;
}

std::vector<Value> NextTTable::solveLeftHelper(std::vector<int> *possibleLefts,
                                               Reference rightRef) {
    std::unordered_set<Value> intermediateResult;
    if (rightRef.isWildcard()) {
        for (int left : *possibleLefts) {
            if (verifyRightWildcard(left)) {
                intermediateResult.insert(
                    Value(ValueType::STMT_NUM, toString(left)));
            }
        }
    } else {
        int right = convertToType<int>(rightRef.getValueString());
        for (int left : *possibleLefts) {
            if (checkNextT(left, right)) {
                intermediateResult.insert(
                    Value(ValueType::STMT_NUM, toString(left)));
            }
        }
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    return result;
}