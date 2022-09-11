#pragma once

#include "PredicateMap.h"
#include "../commons/Relationship/Relationship.h"

template <typename Left, typename Right>
class RelationshipPredicateMap : public PredicateMap<Left, Right> {
public:
	/*
	* Explicit constructor for a RelationshipPredicateMap.
	* Takes in a map specifying the conditions to filter by.
	*/
	explicit RelationshipPredicateMap(std::map<RelationshipHeader, Relationship<Left, Right>*> *predicateMap) {
		this->predicateMap = *predicateMap;
	}

	/*
	* Checks if RelationshipPredicateMap is empty.
	*/
	bool isEmpty() {
		return this->predicateMap.size() == 0;
	}

	/*
	* Gets the map of the predicate.
	*/
	std::map<RelationshipHeader, Relationship<Left, Right>*> getPredicateMap() {
		return this->predicateMap;
	}

private:
	std::map<RelationshipHeader, Relationship<Left, Right>*> predicateMap;
};
