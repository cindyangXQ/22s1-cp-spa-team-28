#pragma once

#include <map>

#include "Entity.h"
#include "Constant.h"
#include "Variable.h"
#include "Procedure.h"
#include "NameDefinitions.h"
#include "PredicateMap.h"

/*
* Classes representing a PredicateMap for filtering NamesTable.
* TODO: make everything into a proper template class EntityPredicateMap if possible
*/
template <typename Name>
class EntityPredicateMap : public PredicateMap<EntityHeader, Entity<Name>> {
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
