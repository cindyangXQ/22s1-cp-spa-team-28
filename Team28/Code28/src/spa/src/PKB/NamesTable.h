#pragma once

#include <map>
#include <type_traits>
#include <unordered_set>

#include "../commons/Entity.h"
#include "EntityPredicateMap.h"
#include "Table.h"

/*
* Class encapsulating a Table used for storing Procedures/Variables/Constants.
*/
template <typename In, typename T> 
class NamesTable : public Table<T> {
	static_assert(std::is_base_of<Entity<In>, T>::value, "T must inherit from Entity");
public:
	/*
	* Stores an entity into NamesTable.
	*/
	void store(T* entity) {
		this->names.insert(entity->getName());
		this->nameEntityMap[entity->getName()] = entity;
		this->tableSize++;
	};

	/*
	* Retrieves an entity from NamesTable by Name.
	*/
	T* retrieve(const In& in) {
		auto key = this->nameEntityMap.find(in);

		if (key == this->nameEntityMap.end()) {
			return nullptr;
		}
		return key->second;
	};

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
	std::map<In, T*> getNameEntityMap() {
		return this->nameEntityMap;
	};

	/*
	* Gets all names from NamesTable.
	*/
	std::unordered_set<In> getAll() {
		return this->names;
	}

private:
	int tableSize = 0;
	std::unordered_set<In> names;
	std::map<In, T*> nameEntityMap;
};

class ConstantsTable : public NamesTable<ConstantName, Constant> {
public:
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

class VariablesTable : public NamesTable<VariableName, Variable> {
public:
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

class ProceduresTable : public NamesTable<ProcedureName, Procedure> {
public:
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
