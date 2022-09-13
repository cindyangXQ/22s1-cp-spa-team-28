#pragma once

#include "Relationship.h"
#include "ValidatorInterface.h"

/*
 * Class encapsulating Follows relationship.
 */
class Uses : Relationship<Reference, Reference>, ValidatorInterface {
public:
    Uses(RelationshipReference relRef, Reference left, Reference right) : Relationship(relRef, left, right) {};
    bool isValid() {
        if (this->getLeft().type == ReferenceType::WILDCARD) {
            return false;
        }
        return (this->getRight().type == ReferenceType::ENT_REF);
    }
};