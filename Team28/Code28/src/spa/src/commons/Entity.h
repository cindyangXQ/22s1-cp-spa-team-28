#pragma once

#include <iostream>

#include "TableValue.h"

enum class EntityHeader {
	NAME
};

enum class EntityName
{
    PROCEDURE,
    STMT,
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
    VARIABLE,
    CONSTANT,
};

/*
* Class encapsulating 1 of the following design entities: Procedure, Constant, Variable.
*/
class Entity : public TableValue {
public:
	explicit Entity(std::string name) {
		this->name = name;
	};

	std::string getName() {
		return this->name;
	};

	bool operator==(const Entity& other) const {
		return this->name == other.name;
	};

	/*
	* Checks whether value of EntityHeader is equal.
	*/
	bool isValueEqual(EntityHeader header, const Entity& other) {
		switch (header) {
			case EntityHeader::NAME:
				return this->name == other.name;
			
			default:
				std::cerr << "Unknown EntityHeader." << std::endl;
		}
	};

private:
	std::string name;
};
