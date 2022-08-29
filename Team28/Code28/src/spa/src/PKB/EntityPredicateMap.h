#pragma once

#include <map>

#include "Entity.h"
#include "PredicateMap.h"

/*
* Class representing a PredicateMap for filtering NamesTable
*/
template <typename Name>
class EntityPredicateMap : public PredicateMap<EntityHeader, Entity<Name>> {
public:
	/*
	* Explicit constructor for a PredicateMap.
	*/
	explicit EntityPredicateMap(std::map<EntityHeader, Entity<Name>*> *predicateMap);
	
	/*
	* Checks if PredicateMap is empty
	*/
    bool isEmpty();

	/*
	* Gets the map of the predicate
	*/
    std::map<EntityHeader, Entity<Name>*> getPredicateMap();
    
    private:
        std::map<EntityHeader, Entity<Name>*> predicateMap;
};