#pragma once

#include "Relationship.h"
#include "ValidatorInterface.h"

/*
 * Class encapsulating Follows relationship.
 */
class ParentT : Relationship<Reference, Reference>, ValidatorInterface {
public:
    ParentT(RelationshipReference relRef, Reference left, Reference right) : Relationship(relRef, left, right) {};
    bool isValid() {
        return (this->getLeft().type == ReferenceType::STMT_REF && this->getRight().type == ReferenceType::STMT_REF);
    }
};