#pragma once

#include "RelationshipsTable.h"

class StmtToNameRelationshipsTable
    : public RelationshipsTable<int, std::string> {
public:
    std::vector<Value> getMatchingValue(std::string value, EntityName entity);
    std::map<Value, std::vector<Value>> getAllValues(EntityName entity);
};
