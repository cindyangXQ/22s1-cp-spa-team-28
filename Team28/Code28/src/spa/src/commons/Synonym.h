#pragma once

#include "../QPS/Error/SemanticError.h"
#include "Entity.h"
#include <iostream>
#include <vector>

/*
 * Class encapsulating a synonym.
 */
class Synonym {
public:
    Synonym(){};
    Synonym(EntityName entity, std::string name);
    std::string getName();
    EntityName getEntityName();
    bool operator==(const Synonym &other) const {
        return this->entity == other.entity && this->name == other.name;
    };

    static Synonym getSynonym(std::string input, std::vector<Synonym> syns);
    static bool isDuplicateSynonymName(std::vector<Synonym> syns);

private:
    EntityName entity;
    std::string name;
};
