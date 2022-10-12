#include "SelectClause.h"

SelectClause::SelectClause() {}
SelectClause::SelectClause(std::vector<Reference> refs, SelectType selectType) {
    this->refs = refs;
    this->selectType = selectType;
}
