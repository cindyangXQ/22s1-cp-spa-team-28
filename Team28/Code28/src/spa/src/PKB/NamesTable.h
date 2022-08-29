#pragma once

#include <map>
#include <type_traits>
#include <unordered_set>

#include "Entity.h"
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
	* Filters table based on the PredicateMap
	*/
	NamesTable<In, T> *filter(EntityPredicateMap<T> *predicateMap) {
		if ((*predicateMap).isEmpty()) {
			NamesTable<In, T> *newTable = this;
			return newTable;
		}

		NamesTable<In, T> newTable;
		std::map<EntityHeader, T> extractedMap = (*predicateMap).getPredicateMap();

		for (auto [name, entity] : this->nameEntityMap) {
			bool isFilter = true;
			for (auto [key, val] : extractedMap) {
				if (!entity->isValueEqual(*key, val)) {
					isFilter = false;
				}
			}
			if (isFilter) {
				newTable.store(entity);
			}
		}
	};

private:
	int tableSize = 0;
	std::unordered_set<In> names;
	std::map<In, T*> nameEntityMap;
};