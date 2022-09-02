#include "StatementPredicateMap.h"

StatementPredicateMap::StatementPredicateMap(std::map<StatementHeader, Statement*> *predicateMap) {
    this->predicateMap = *predicateMap;
};

bool StatementPredicateMap::isEmpty() {
    return this->predicateMap.size() == 0;
};

std::map<StatementHeader, Statement*> StatementPredicateMap::getPredicateMap() {
    return this->predicateMap;
};
