#pragma once

#include <map>

#include "Entity.h"
#include "PredicateMap.h"

enum class EntityHeader {
	NAME
};

/*
* Class representing a PredicateMap for filtering NamesTable
*/
template <typename Name>
class EntityPredicateMap : public PredicateMap<EntityHeader, Entity<Name> {
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