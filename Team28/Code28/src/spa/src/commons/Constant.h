#pragma once 

#include <string>

#include "Entity.h"
#include "NameDefinitions.h"

/*
* Class encapsulating a SIMPLE constant value.
*/
class Constant : public Entity<ConstantName> {
public:
	explicit Constant(ConstantName name);
};
