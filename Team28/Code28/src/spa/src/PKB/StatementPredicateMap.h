#pragma once

#include <map>

#include "Statement.h"
#include "PredicateMap.h"

/*
* Class representing a PredicateMap for filtering StatementsTable
*/
class StatementPredicateMap : public PredicateMap<StatementHeader, Statement> {
public:
	/*
	* Explicit constructor for a PredicateMap.
	* TODO: Document map
	*/
	explicit StatementPredicateMap(std::map<StatementHeader, Statement*> *predicateMap);

	/*
	* Checks if PredicateMap is empty
	*/
    bool isEmpty();

	/*
	* Gets the map of the predicate
	*/
    std::map<StatementHeader, Statement*> getPredicateMap();
    
    private:
        std::map<StatementHeader, Statement*> predicateMap;
};
