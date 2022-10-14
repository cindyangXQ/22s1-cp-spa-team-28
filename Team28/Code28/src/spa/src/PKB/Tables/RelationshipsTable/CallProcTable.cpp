#include "CallProcTable.h"

std::vector<Value> CallProcTable::getValue(std::string value,
                                           EntityName entity) {
    std::unordered_set<int> stmts = retrieveRight(value);
    std::vector<Value> result = {};
    for (int i : stmts) {
        result.push_back(Value(ValueType::STMT_NUM, toString(i)));
    }
    return result;
};