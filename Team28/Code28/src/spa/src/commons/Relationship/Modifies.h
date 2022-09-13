#pragma once

#include "Relationship.h"

/*
 * Class encapsulating Modifies relationship.
 */
class Modifies : Relationship<Reference, Reference> {
public:
    Modifies(RelationshipReference relRef, Reference left, Reference right) : Relationship(relRef, left, right) {};
    bool isValid() {
        if (this->getLeft().type == ReferenceType::WILDCARD) {
            return false;
        }
        return this->getRight().type == ReferenceType::ENT_REF;
    }
    ClauseResult evaluate() {
        return ClauseResult(false);
    }
};