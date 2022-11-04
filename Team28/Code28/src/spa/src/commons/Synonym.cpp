#include "Synonym.h"

Synonym::Synonym(EntityName entity, std::string name) {
    this->entity = entity;
    this->name = name;
}
std::string Synonym::getName() { return this->name; }

EntityName Synonym::getEntityName() { return this->entity; }

Synonym Synonym::getSynonym(std::string input, std::vector<Synonym> syns) {
    for (int i = 0; i < syns.size(); i++) {
        Synonym synonym = syns[i];
        if (input.compare(synonym.getName()) == 0) {
            return synonym;
        }
    }
    throw SemanticError("Synonym not found");
}

bool Synonym::isDuplicateSynonymName(std::vector<Synonym> syns) {
    std::vector<std::string> names;
    int i, j;
    for (i = 0; i < syns.size(); i++) {
        names.push_back(syns[i].getName());
    }
    for (i = 0; i < syns.size(); i++) {
        for (j = 0; j < syns.size(); j++) {
            if (i == j) {
                continue;
            } else if (syns[i].getName() == names[j]) {
                return true;
            }
        }
    }
    return false;
}
