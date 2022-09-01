#pragma once
#include <string>
#include "Reference.h"

class VariableName : public Reference {
public:
	std::string varName;
	VariableName(std::string name) {
		this->varName = name;
	}
};