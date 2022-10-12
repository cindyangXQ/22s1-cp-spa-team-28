#pragma once

#include "../../commons/Synonym.h"
#include <vector>

enum class SelectType { SINGLE, TUPLE, BOOLEAN };
/*
 * Class encapsulating the logic of the select clause.
 */
class SelectClause {
public:
    std::vector<Synonym> syns;
    SelectType selectType;
    SelectClause();
    SelectClause(std::vector<Synonym> syns, SelectType selectType);
};
