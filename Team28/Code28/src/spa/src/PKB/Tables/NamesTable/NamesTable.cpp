#include "NamesTable.h"
#include "../../../commons/StringUtil.h"

std::vector<int> ProceduresTable::getAllStmtNum() {
    std::vector<int> stmts;

    for (auto const &[key, val] : this->nameEntityMap) {
        stmts.push_back(val->getStmtNum());
    }
    return stmts;
}
