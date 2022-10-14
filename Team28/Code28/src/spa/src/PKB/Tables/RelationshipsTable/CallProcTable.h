#pragma once

#include "RelationshipsTable.h"

class CallProcTable : public RelationshipsTable<int, std::string> {
public:
    std::vector<Value> getValue(std::string value, EntityName entity);
};