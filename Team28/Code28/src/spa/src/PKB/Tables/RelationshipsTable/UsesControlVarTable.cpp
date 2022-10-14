#include "UsesControlVarTable.h"

UsesControlVarTable::UsesControlVarTable() = default;

std::vector<Value> UsesControlVarTable::getAllStmts() {
    if (this->isProcessed) {
        return this->allStmts;
    }

    for (auto const &[key, val] : this->leftToRightsMap) {
        this->allStmts.push_back(
            Value(ValueType::STMT_NUM, std::to_string(key)));
    }
    std::sort(this->allStmts.begin(), this->allStmts.end());
    this->isProcessed = true;
    return this->allStmts;
}

std::vector<Value> UsesControlVarTable::getStmt(std::string varName) {
    if (varName == "_") {
        return this->getAllStmts();
    }
    std::unordered_set<Value> intermediateResult;
    std::unordered_set<int> statements = this->retrieveRight(varName);
    for (int stmt : statements) {
        intermediateResult.insert(
            Value(ValueType::STMT_NUM, std::to_string(stmt)));
    }
    std::vector<Value> result = std::vector<Value>(intermediateResult.begin(),
                                                   intermediateResult.end());
    std::sort(result.begin(), result.end());
    return result;
};

std::vector<std::pair<Value, Value>> UsesControlVarTable::getStmtAndVar() {
    std::unordered_set<std::pair<Value, Value>, value_pair_hash>
        intermediateResult;

    for (auto const &[key, val] : this->leftToRightsMap) {
        for (std::string var : val) {
            intermediateResult.insert(
                std::make_pair(Value(ValueType::STMT_NUM, std::to_string(key)),
                               Value(ValueType::VAR_NAME, var)));
        }
    }
    std::vector<std::pair<Value, Value>> result =
        std::vector<std::pair<Value, Value>>(intermediateResult.begin(),
                                             intermediateResult.end());
    std::sort(result.begin(), result.end(), value_pair_sort());
    return result;
};

std::vector<Value> UsesControlVarTable::getMatchingValue(std::string value,
                                                         EntityName entity) {
    UNUSED(value);
    UNUSED(entity);
    return std::vector<Value>{};
};

std::map<Value, std::vector<Value>>
UsesControlVarTable::getAllValues(EntityName entity) {
    UNUSED(entity);
    return std::map<Value, std::vector<Value>>{};
};