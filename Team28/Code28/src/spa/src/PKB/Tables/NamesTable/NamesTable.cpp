#include "NamesTable.h"
#include "../../../commons/StringUtil.h"

std::vector<Value> ConstantsTable::getValue(int value, EntityName entity) {
    std::string v = toString(value);
    std::vector<Value> result = {};
    if (this->names.count(v) == 1) {
        result.push_back(Value(ValueType::STMT_NUM, v));
    }
    return result;
}

std::vector<Value> VariablesTable::getValue(std::string value,
                                            EntityName entity) {
    std::vector<Value> result = {};
    if (this->names.count(value) == 1) {
        result.push_back(Value(ValueType::VAR_NAME, value));
    }
    return result;
}

std::vector<Value> ProceduresTable::getValue(std::string value,
                                             EntityName entity) {
    std::vector<Value> result = {};
    if (this->names.count(value) == 1) {
        result.push_back(Value(ValueType::VAR_NAME, value));
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
