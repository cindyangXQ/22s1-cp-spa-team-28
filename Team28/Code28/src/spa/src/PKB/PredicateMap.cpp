#include "PredicateMap.h"

template <typename T, typename U>
PredicateMap<T,U>::PredicateMap(std::map<T*, U*> predicateMap) {
    this->predicateMap = predicateMap;
};

// Checks if predicate is empty, 
template <typename T,typename U>
bool PredicateMap<T, U>::isEmpty() {
    return this->predicateMap.size() == 0;
};

template <typename T, typename U>
std::map<T*, U*> PredicateMap<T,U>::getPredicateMap() {
    return this->predicateMap;
};