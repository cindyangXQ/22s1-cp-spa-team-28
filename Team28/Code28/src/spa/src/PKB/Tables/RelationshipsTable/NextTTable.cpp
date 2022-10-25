#include "NextTTable.h"

std::vector<Value> NextTTable::solveBothReflexive(EntityName stmtEntity,
                                                  StorageView *storage) {
    StatementType stmtType = Statement::getStmtTypeFromEntityName(stmtEntity);
    StatementsTable *statements = storage->getTable<StatementsTable>();
    std::vector<int> possibleValues = statements->getStatementsByType(stmtType);
    std::unordered_set<int> setOfPossibleValues(possibleValues.begin(),
                                                possibleValues.end());

    std::vector<Value> result = {};

    for (auto const &[key, val] : this->getLeftMap()) {
        if (setOfPossibleValues.count(key) > 0) {
            if (val.count(key) > 0) {
                Value currVal = Value(ValueType::STMT_NUM, toString(key));
                result.push_back(currVal);
            }
        }
    }
    return result;
}
