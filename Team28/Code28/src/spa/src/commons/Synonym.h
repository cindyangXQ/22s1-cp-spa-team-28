#pragma once

#include "Entity.h"
#include <iostream>

// private all fields

/*
 * Class encapsulating a synonym.
 */
class Synonym {
public:
    Synonym();
    Synonym(EntityName entity, std::string name);
    std::string getName();
    EntityName getEntityName();

private:
    EntityName entity;
    std::string name;
};
