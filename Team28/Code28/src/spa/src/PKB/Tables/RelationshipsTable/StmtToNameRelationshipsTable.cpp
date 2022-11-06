#include "StmtToNameRelationshipsTable.h"

std::vector<Value> StmtToNameRelationshipsTable::getMatchingValue(
    std::string value, EntityName entity, StorageView *storage) {
    std::unordered_set<int> stmts = retrieveRight(value);
    std::unordered_set<int> possibleStmts = getPossibleStmts(entity, storage);
    std::vector<Value> result = {};
    for (int i : stmts) {
        if (possibleStmts.count(i) > 0) {
            result.push_back(Value(ValueType::STMT_NUM, toString(i)));
        }
    }
    return result;
};

std::map<Value, std::vector<Value>>
StmtToNameRelationshipsTable::getAllValues(EntityName entity,
                                           StorageView *storage) {
    std::map<Value, std::vector<Value>> result = {};
    std::unordered_set<int> possibleStmts = getPossibleStmts(entity, storage);
    for (auto const &[key, val] : this->rightToLeftsMap) {
        Value rValue = Value(ValueType::VAR_NAME, key);
        result[rValue] = {};
        for (int stmt : val) {
            if (possibleStmts.count(stmt) > 0) {
                Value lValue = Value(ValueType::STMT_NUM, toString(stmt));
                result[rValue].push_back(lValue);
            }
        }
    }
    return result;
};

std::unordered_set<int>
StmtToNameRelationshipsTable::getPossibleStmts(EntityName entity,
                                               StorageView *storage) {
    StatementsTable *statements = storage->getTable<StatementsTable>();
    return statements->getStatementsSetByType(
        Statement::getStmtTypeFromEntityName(entity));
};
