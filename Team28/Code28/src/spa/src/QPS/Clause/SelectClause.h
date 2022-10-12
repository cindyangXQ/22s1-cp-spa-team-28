#pragma once

#include "../../commons/Reference.h"
#include <vector>

enum class SelectType { SINGLE, TUPLE, BOOLEAN };
/*
 * Class encapsulating the logic of the select clause.
 */
class SelectClause {
public:
    std::vector<Reference> refs;
    SelectType selectType;
    SelectClause();
    SelectClause(std::vector<Reference> refs, SelectType selectType);
};
