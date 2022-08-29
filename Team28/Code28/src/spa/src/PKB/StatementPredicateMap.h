#pragma once

#include <map>

#include "Statement.h"
#include "PredicateMap.h"

enum class StatementHeader {
	NAME
};

/*
* Class representing a PredicateMap for filtering StatementsTable
*/
template <typename Name>
class StatementPredicateMap : public PredicateMap<EntityHeader, Entity<Name> {
public:
	/*
	* Explicit constructor for a Predicate.
	*/
	explicit PredicateMap(std::map<EntityHeader, Entity<Name>> predicateMap);

	/*
	* Gets the map of the predicate
	*/
    std::map<EntityHeader, Entity<Name>> getPredicateMap();
    
    private:
        std::map<EntityHeader, Entity<Name>> predicateMap;
};