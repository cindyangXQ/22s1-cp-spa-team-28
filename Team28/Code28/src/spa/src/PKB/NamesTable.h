#pragma once

#include <map>
#include <string>
#include <type_traits>
#include <unordered_set>

#include "../commons/Entity.h"
#include "EntityPredicateMap.h"
#include "Table.h"

/*
* Class encapsulating a Table used for storing Procedures/Variables/Constants.
*/
class NamesTable : public Table<Entity> {
public:
	/*
	* Stores an entity into NamesTable.
	*/
	void store(Entity* entity) {
		this->names.insert(entity->getName());
		this->nameEntityMap[entity->getName()] = entity;
		this->tableSize++;
	};

	/*
	* Retrieves an entity from NamesTable by Name.
	*/
	virtual Entity* retrieve(const std::string& name) = 0;

	/*
	* Returns the size of NamesTable.
	*/
	int getTableSize() const {
		return this->tableSize;
	};

	/*
	* Gets NamedEntityMap for children. 
	* TODO: Consider removing.
	*/
	std::map<std::string, Entity*> getNameEntityMap() {
		return this->nameEntityMap;
	};

	/*
	* Gets all names from NamesTable.
	*/
	std::unordered_set<std::string> getAll() {
		return this->names;
	}

protected:
	int tableSize = 0;
	std::unordered_set<std::string> names;
	std::map<std::string, Entity*> nameEntityMap;
};

class ConstantsTable : public NamesTable {
public:
	virtual Constant* retrieve(const std::string& name) {
		auto key = this->nameEntityMap.find(name);

		if (key == this->nameEntityMap.end()) {
			return nullptr;
		}
		return (Constant*) key->second;
	};

	/*
	* Filter table based on the PredicateMap.
	*/
	ConstantsTable *filter(ConstantPredicateMap *predicateMap) {
		if ((*predicateMap).isEmpty()) {
			ConstantsTable *newTable = this;
			return newTable;
		}

		ConstantsTable *newTable = new ConstantsTable();
		std::map<EntityHeader, Constant*> extractedMap = (*predicateMap).getPredicateMap();

		for (auto const&[name, entity] : this->getNameEntityMap()) {
			bool isFilter = true;
			for (auto const&[key, val] : extractedMap) {
				if (!entity->isValueEqual(key, *val)) {
					isFilter = false;
				}
			}
			if (isFilter) {
				newTable->store(entity);
			}
		}
		return newTable;
	};
};

class VariablesTable : public NamesTable {
public:
	virtual Variable* retrieve(const std::string& name) {
		auto key = this->nameEntityMap.find(name);

		if (key == this->nameEntityMap.end()) {
			return nullptr;
		}
		return (Variable*) key->second;
	};

	/*
	* Filter table based on the PredicateMap.
	*/
	VariablesTable *filter(VariablePredicateMap *predicateMap) {
		if ((*predicateMap).isEmpty()) {
			VariablesTable *newTable = this;
			return newTable;
		}

		VariablesTable *newTable = new VariablesTable();
		std::map<EntityHeader, Variable*> extractedMap = (*predicateMap).getPredicateMap();

		for (auto const&[index, entity] : this->getNameEntityMap()) {
			bool isFilter = true;
			for (auto const&[key, val] : extractedMap) {
				if (!entity->isValueEqual(key, *val)) {
					isFilter = false;
				}
			}
			if (isFilter) {
				newTable->store(entity);
			}
		}
		return newTable;
	};
};

class ProceduresTable : public NamesTable {
public:
	virtual Procedure* retrieve(const std::string& name) {
		auto key = this->nameEntityMap.find(name);

		if (key == this->nameEntityMap.end()) {
			return nullptr;
		}
		return (Procedure*) key->second;
	};
	/*
	* Filter table based on the PredicateMap.
	*/
	ProceduresTable *filter(ProcedurePredicateMap *predicateMap) {
		if ((*predicateMap).isEmpty()) {
			ProceduresTable *newTable = this;
			return newTable;
		}

		ProceduresTable *newTable = new ProceduresTable();
		std::map<EntityHeader, Procedure*> extractedMap = (*predicateMap).getPredicateMap();

		for (auto const&[name, entity] : this->getNameEntityMap()) {
			bool isFilter = true;
			for (auto const&[key, val] : extractedMap) {
				if (!entity->isValueEqual(key, *val)) {
					isFilter = false;
				}
			}
			if (isFilter) {
				newTable->store(entity);
			}
		}
		return newTable;
	};
};
