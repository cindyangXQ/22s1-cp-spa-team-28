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
    this->isProcessed = true;
    return this->allStmts;
}

std::vector<Value> UsesControlVarTable::getStmt(std::string varName) {
    if (varName == WILDCARD_SYMBOL) {
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
    return result;
};

std::vector<Value> UsesControlVarTable::getMatchingValue(std::string value,
                                                         EntityName entity,
                                                         StorageView *storage) {
    UNUSED(value);
    UNUSED(entity);
    UNUSED(storage);
    return std::vector<Value>{};
};

std::map<Value, std::vector<Value>>
UsesControlVarTable::getAllValues(EntityName entity, StorageView *storage) {
    UNUSED(entity);
    UNUSED(storage);
    return std::map<Value, std::vector<Value>>{};
};

bool UsesControlVarTable::validate(int stmtNo, std::string varName) {
    if (this->leftToRightsMap.count(stmtNo) == 0) {
        return false;
    }
    std::unordered_set<std::string> vars = this->leftToRightsMap.at(stmtNo);
    return vars.count(varName) != 0;
}

std::vector<Value> UsesControlVarTable::getVar(int stmtNo) {
    if (this->leftToRightsMap.count(stmtNo) == 0) {
        return std::vector<Value>();
    }
    std::unordered_set<std::string> vars = this->leftToRightsMap.at(stmtNo);
    std::vector<Value> result = {};
    for (std::string var : vars) {
        result.push_back(Value(ValueType::VAR_NAME, var));
    }
    return result;
}
