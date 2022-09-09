#pragma once

#include <map>

#include "../commons/Entity.h"
#include "../commons/Constant.h"
#include "../commons/Variable.h"
#include "../commons/Procedure.h"
#include "PredicateMap.h"

/*
* Classes representing a PredicateMap for filtering NamesTable.
* TODO: make everything into a proper template class EntityPredicateMap if possible
*/
template<typename T>
class EntityPredicateMap : public PredicateMap<EntityHeader, Entity> {
public:
	/*
	* Explicit constructor for a PredicateMap.
	*/
	explicit EntityPredicateMap(std::map<EntityHeader, T*> *predicateMap) {
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
    virtual std::map<EntityHeader, T*> getPredicateMap() {
		return this->predicateMap;
	};
    
    protected:
        std::map<EntityHeader, T*> predicateMap;
};

class ConstantPredicateMap : public EntityPredicateMap<Constant> {

};

class VariablePredicateMap : public EntityPredicateMap<Variable> {

};

class ProcedurePredicateMap : public EntityPredicateMap<Procedure> {

};
