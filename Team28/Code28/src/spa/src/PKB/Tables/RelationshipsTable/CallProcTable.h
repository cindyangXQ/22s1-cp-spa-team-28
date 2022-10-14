#pragma once

#include "RelationshipsTable.h"

class CallProcTable : public RelationshipsTable<int, std::string> {
public:
    std::vector<Value> getMatchingValue(std::string value, EntityName entity);
    std::map<Value, std::vector<Value>> getAllValues(EntityName entity);
};