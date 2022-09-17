#include "Synonym.h"

Synonym::Synonym() {}
Synonym::Synonym(EntityName entity, std::string name) {
    this->entity = entity;
    this->name = name;
}
