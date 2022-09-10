#pragma once

#include "Relationship.h"

/*
 * Class encapsulating Follows relationship.
 */
class Follows : Relationship<Reference, Reference> {
public:
    Follows(RelationshipReference relRef, Reference left, Reference right) : Relationship(relRef, left, right) {};
    bool isValid() {
        return (this->getLeft().type == ReferenceType::STMT_REF && this->getRight().type == ReferenceType::STMT_REF);
    }
};