#pragma once 

#include <string>

#include "Entity.h"
#include "Reference.h"

/*
* Class encapsulating a SIMPLE variable.
*/
class Variable : public Entity, public Reference {
public:
	explicit Variable(std::string name);
};
