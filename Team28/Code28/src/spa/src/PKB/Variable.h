#pragma once 

#include <string>

#include "Entity.h"
#include "NameDefinitions.h"

/*
* Class encapsulating a SIMPLE variable.
*/
class Variable : public Entity<VariableName> {
public:
	explicit Variable(VariableName name);
};
