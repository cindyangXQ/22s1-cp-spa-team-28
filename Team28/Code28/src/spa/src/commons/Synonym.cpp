#include "Synonym.h"

Synonym::Synonym() {}
Synonym::Synonym(EntityName entity, std::string name) {
    this->entity = entity;
    this->name = name;
}
std::string Synonym::getName() { return this->name; }

EntityName Synonym::getEntityName() { return this->entity; }