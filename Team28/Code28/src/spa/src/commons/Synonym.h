#pragma once

#include "Entity.h"
#include <iostream>

/*
 * Class encapsulating a synonym.
 */
class Synonym {
public:
    EntityName entity;
    std::string name;
    Synonym();
    Synonym(EntityName entity, std::string name);
};
