#pragma once

#include <map>

#include "../commons/Entity.h"
#include "../commons/Constant.h"
#include "../commons/Variable.h"
#include "../commons/Procedure.h"
#include "PredicateMap.h"

/*
* Classes representing a PredicateMap for filtering NamesTable.
*/
template <typename T>
class EntityPredicateMap : public PredicateMap<EntityHeader, T> {
public:
	EntityPredicateMap(std::map<EntityHeader, T*> *predicateMap) {
		this->predicateMap = *predicateMap;
	};
	
	/*
	* Checks if PredicateMap is empty.
	*/
    bool isEmpty() {
		return this->predicateMap.size() == 0;
	};

	/*
	* Gets the map of the predicate.
	*/
    std::map<EntityHeader, T*> getPredicateMap() {
		return this->predicateMap;
	};

	protected:
		std::map<EntityHeader, T*> predicateMap;
};

class ConstantPredicateMap : public EntityPredicateMap<Constant> {
public:
	explicit ConstantPredicateMap(std::map<EntityHeader, Constant*> *predicateMap) : EntityPredicateMap(predicateMap) {};
};

class VariablePredicateMap : public EntityPredicateMap<Variable> {
public:
	explicit VariablePredicateMap(std::map<EntityHeader, Variable*> *predicateMap) : EntityPredicateMap(predicateMap) {};
};

class ProcedurePredicateMap : public EntityPredicateMap<Procedure> {
public:
	explicit ProcedurePredicateMap(std::map<EntityHeader, Procedure*> *predicateMap) : EntityPredicateMap(predicateMap) {};
};