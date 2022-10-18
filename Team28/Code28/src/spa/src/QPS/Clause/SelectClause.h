#pragma once

#include "../../commons/Reference.h"
#include <vector>

enum class SelectType { SINGLE, TUPLE, BOOLEAN };
/*
 * Class encapsulating the logic of the select clause.
 */
class SelectClause {
public:
    SelectClause(){};
    SelectClause(std::vector<Reference> refs, SelectType selectType)
        : refs(refs), selectType(selectType){};
    std::vector<Reference> getRefs();
    SelectType getSelectType();

private:
    std::vector<Reference> refs;
    SelectType selectType;
};
