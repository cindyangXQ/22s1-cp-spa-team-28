#include "CallProcTable.h"

std::vector<Value> CallProcTable::getMatchingValue(std::string value,
                                                   EntityName entity) {
    std::unordered_set<int> stmts = retrieveRight(value);
    std::vector<Value> result = {};
    for (int i : stmts) {
        result.push_back(Value(ValueType::STMT_NUM, toString(i)));
    }
    return result;
};

std::map<Value, std::vector<Value>>
CallProcTable::getAllValues(EntityName entity) {
    std::map<Value, std::vector<Value>> result = {};
    for (auto const &[key, val] : this->rightToLeftsMap) {
        Value rValue = Value(ValueType::VAR_NAME, key);
        result[rValue] = {};
        for (int stmt : val) {
            Value lValue = Value(ValueType::STMT_NUM, toString(stmt));
            result[rValue].push_back(lValue);
        }
    }
    return result;
};