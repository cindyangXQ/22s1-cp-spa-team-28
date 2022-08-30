#pragma once
#include "DesignEntity.h"
#include <iostream>

class Synonym
{
public:
	DesignEntity entity;
	std::string name;
    Synonym();
    Synonym(std::string desc);
	Synonym(DesignEntity entity, std::string name);
};