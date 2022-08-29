#include "EntityPredicateMap.h"

template <typename Name>
EntityPredicateMap<Name>::EntityPredicateMap(std::map<EntityHeader, Entity<Name>*> *predicateMap) {
    this->predicateMap = *predicateMap;
};

// Checks if predicate is empty, 
template <typename Name>
bool EntityPredicateMap<Name>::isEmpty() {
    return this->predicateMap.size() == 0;
};

template <typename Name>
std::map<EntityHeader, Entity<Name>*> EntityPredicateMap<Name>::getPredicateMap() {
    return this->predicateMap;
};