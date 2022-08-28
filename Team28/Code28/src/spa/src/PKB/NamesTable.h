#pragma once

#include <map>
#include <type_traits>
#include <unordered_set>

#include "Entity.h"
#include "Table.h"

/*
* Class encapsulating a Table used for storing Procedures/Variables/Constants.
*/
template <typename In, typename T> 
class NamesTable : public Table<T> {
	static_assert(std::is_base_of<Entity<In>, T>::value, "T must inherit from Entity");
public:
	void store(T* entity) {
		this->names.insert(entity->getName());
		this->nameEntityMap[entity->getName()] = entity;
		this->tableSize++;
	};

	T* retrieve(const In& in) {
		auto key = this->nameEntityMap.find(in);

		if (key == this->nameEntityMap.end()) {
			return nullptr;
		}

		return key->second;
	};

	int getTableSize() const {
		return this->tableSize;
	};

private:
	int tableSize = 0;
	std::unordered_set<In> names;
	std::map<In, T*> nameEntityMap;
};