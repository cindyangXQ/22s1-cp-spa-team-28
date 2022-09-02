#pragma once
#include <string>
#include "Reference.h"

/*
 * Class encapsulating a variable name.
 */
class VariableName : public Reference {
public:
	std::string varName;
	VariableName(std::string name) {
		this->varName = name;
	}
};