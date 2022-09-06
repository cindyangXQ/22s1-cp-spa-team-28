#pragma once 

#include <string>

#include "Entity.h"
#include "NameDefinitions.h"
#include "Reference.h"

/*
* Class encapsulating a SIMPLE variable.
*/
class Variable : public Entity<VariableName>, public Reference {
public:
	explicit Variable(VariableName name);
};
