#pragma once

#include <map>

/*
* Class representing a PredicateMap to be used for filtering Tables
*/
template <typename T, typename U>
class PredicateMap {
public:
	/*
	* Explicit constructor for a Predicate.
	*/
    explicit PredicateMap(std::map<T*, U*> predicateMap);

	/*
	* Checks if Predicate is empty
	*/
    bool isEmpty();

	/*
	* Gets the map of the predicate
	*/
    std::map<T*, U*> getPredicateMap();
    
    private:
        std::map<T*, U*> predicateMap;
};