#include "NamesTable.h"
#include "../../../commons/StringUtil.h"

std::vector<Value> ConstantsTable::getMatchingValue(std::string value,
                                                    EntityName entity) {
    UNUSED(entity);
    std::vector<Value> result = {};
    if (this->names.count(value) == 1) {
        result.push_back(Value(ValueType::STMT_NUM, value));
    }
    return result;
}

std::map<Value, std::vector<Value>>
ConstantsTable::getAllValues(EntityName entity) {
    UNUSED(entity);
    std::map<Value, std::vector<Value>> result = {};
    for (std::string name : this->names) {
        Value v = Value(ValueType::STMT_NUM, name);
        result[v] = {v};
    }
    return result;
}

std::vector<int> ProceduresTable::getAllStmtNum() {
    std::vector<int> stmts;

    for (auto const &[key, val] : this->nameEntityMap) {
        stmts.push_back(val->getStmtNum());
    }
    return stmts;
}
