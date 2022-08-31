#pragma once
#include <string>
#include "EntityReference.h"

class VariableName : public EntityReference {
public:
	std::string varName;
	VariableName(std::string name) {
		this->varName = name;
	}
};