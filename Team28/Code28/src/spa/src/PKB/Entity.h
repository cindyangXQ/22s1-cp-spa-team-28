#pragma once

#include <iostream>

#include "TableValue.h"

enum class EntityHeader {
	NAME
};

/*
* Class encapsulating 1 of the following design entities: Procedure, Constant, Variable.
*/
template <typename Name>
class Entity : public TableValue {
public:
	explicit Entity(Name name) {
		this->name = name;
	};

	Name getName() {
		return this->name;
	};

	bool operator==(const Entity& other) const {
		return this->name == other.name;
	};

	/*
	* Checks whether value of EntityHeader is equal
	*/
	bool isValueEqual(EntityHeader header, const Entity& other) {
		switch (header) {
			case EntityHeader::NAME:
				return isNameEqual(other);
			
			default:
				std::cerr << "Unknown EntityHeader." << std::endl;
		}
	};

	bool isNameEqual(const Entity& other) {
		// Duplicate code, to remove if EntityHeader only has one enum
		return this->name == other.name;
	};

private:
	Name name;
};
