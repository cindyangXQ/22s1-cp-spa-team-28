#include "SelectClause.h"

SelectClause::SelectClause() {}
SelectClause::SelectClause(std::vector<Synonym> syns, SelectType selectType) {
    this->syns = syns;
    this->selectType = selectType;
}