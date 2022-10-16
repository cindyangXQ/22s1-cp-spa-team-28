#include "SelectClause.h"

std::vector<Reference> SelectClause::getRefs() {
    return this->refs;
}
SelectType SelectClause::getSelectType() {
    return this->selectType;
}