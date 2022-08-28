#pragma once 

#include <string>

#include "Entity.h"
#include "NameDefinitions.h"

class Constant : public Entity<ConstantName> {
public:
	explicit Constant(ConstantName name);
};
