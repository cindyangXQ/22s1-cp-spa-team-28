#pragma once

#include "../Attributable.h"
#include "RelationshipsTable.h"

class StmtToNameRelationshipsTable
    : public RelationshipsTable<int, std::string>,
      public Attributable {
public:
    std::vector<Value> getMatchingValue(std::string value, EntityName entity,
                                        StorageView *storage);
    std::map<Value, std::vector<Value>> getAllValues(EntityName entity,
                                                     StorageView *storage);

private:
    std::unordered_set<int> getPossibleStmts(EntityName entity,
                                             StorageView *storage);
};
