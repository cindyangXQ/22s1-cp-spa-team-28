#pragma once
#include "../../commons/Entity.h"
#include "../../commons/Reference.h"
#include "../Utils.h"
#include <iostream>

/*
 * Class encapsulating a synonym.
 */
class Synonym : public Reference
{
public:
	EntityName entity;
	std::string name;
    Synonym();
	Synonym(EntityName entity, std::string name);
};