#pragma once
#include "DesignEntity.h"
#include "Reference.h"
#include "../Utils.h"
#include <iostream>

class Synonym : public Reference
{
public:
	DesignEntity entity;
	std::string name;
    Synonym();
	Synonym(DesignEntity entity, std::string name);
};