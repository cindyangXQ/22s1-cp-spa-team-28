#pragma once

#include "Relationship.h"

/*
 * Class encapsulating Follows relationship.
 */
class Follows : Relationship<Reference, Reference> {
public:
    Follows(RelationshipReference relRef, Reference left, Reference right) : Relationship(relRef, left, right) {};
    bool isValid() {
        if (this->getLeft().type == ReferenceType::WILDCARD) {
            return false;
        }
        return (this->getLeft().type == ReferenceType::STMT_REF && this->getRight().type == ReferenceType::STMT_REF);
    }
    ClauseResult evaluate() {
        return ClauseResult(false);
    }
};