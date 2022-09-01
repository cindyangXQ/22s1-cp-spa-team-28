#include "StatementPredicateMap.h"

StatementPredicateMap::StatementPredicateMap(std::map<StatementHeader, Statement*> *predicateMap) {
    this->predicateMap = *predicateMap;
};

// Checks if predicate is empty, 
bool StatementPredicateMap::isEmpty() {
    return this->predicateMap.size() == 0;
};

std::map<StatementHeader, Statement*> StatementPredicateMap::getPredicateMap() {
    return this->predicateMap;
};
