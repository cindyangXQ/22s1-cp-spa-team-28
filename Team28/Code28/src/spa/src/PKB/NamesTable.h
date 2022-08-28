#pragma once

#include <unordered_set>
#include <map>

#include "Entity.h"
#include "Table.h"

template <typename In, typename T> // example use is <ProcedureName, Procedure>
class NamesTable : public Table<T> {
public:
	void store(T* entity) {
		this->names.insert(entity->getName());
		this->nameEntityMap[entity->getName()] = entity;
		this->tableSize++;
	};

	T* retrieve(In& in) {
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