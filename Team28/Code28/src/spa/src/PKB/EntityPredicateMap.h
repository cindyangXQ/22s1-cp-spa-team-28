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
template <typename T>
class EntityPredicateMap : public PredicateMap<EntityHeader, T> {
public:
	virtual bool isEmpty() = 0;
	// virtual std::map<EntityHeader, Entity<Name>*> getPredicateMap() = 0; 
};

class ConstantPredicateMap : public EntityPredicateMap<Constant> {
public:
	/*
	* Explicit constructor for a PredicateMap.
	*/
	explicit ConstantPredicateMap(std::map<EntityHeader, Constant*> *predicateMap) {
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
    std::map<EntityHeader, Constant*> getPredicateMap() {
		return this->predicateMap;
	};
    
    private:
        std::map<EntityHeader, Constant*> predicateMap;
};

class VariablePredicateMap : public EntityPredicateMap<Variable> {
public:
	/*
	* Explicit constructor for a PredicateMap.
	*/
	explicit VariablePredicateMap(std::map<EntityHeader, Variable*> *predicateMap) {
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
    std::map<EntityHeader, Variable*> getPredicateMap() {
		return this->predicateMap;
	};
    
    private:
        std::map<EntityHeader, Variable*> predicateMap;
};

class ProcedurePredicateMap : public EntityPredicateMap<Procedure> {
public:
	/*
	* Explicit constructor for a PredicateMap.
	*/
	explicit ProcedurePredicateMap(std::map<EntityHeader, Procedure*> *predicateMap) {
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
    std::map<EntityHeader, Procedure*> getPredicateMap() {
		return this->predicateMap;
	};
    
    private:
        std::map<EntityHeader, Procedure*> predicateMap;
};