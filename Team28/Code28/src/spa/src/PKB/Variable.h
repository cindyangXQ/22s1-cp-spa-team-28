#pragma once 

#include <string>

#include "Entity.h"
#include "NameDefinitions.h"

class Variable : public Entity<VariableName> {
public:
	explicit Variable(VariableName name);
};
