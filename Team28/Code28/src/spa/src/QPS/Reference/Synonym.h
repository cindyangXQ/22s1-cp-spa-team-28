#pragma once
#include "DesignEntity.h"
#include "StatementReference.h"
#include "EntityReference.h"
#include <iostream>

class Synonym : StatementReference, EntityReference
{
public:
	DesignEntity entity;
	std::string name;
    Synonym();
    Synonym(std::string desc);
	Synonym(DesignEntity entity, std::string name);
};