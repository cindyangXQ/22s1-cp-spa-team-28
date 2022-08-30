#include "Synonym.h"
#include "../Utils.h"

Synonym::Synonym() {}
Synonym::Synonym(DesignEntity entity, std::string name) {
	this->entity = entity;
    this->name = name;
}
