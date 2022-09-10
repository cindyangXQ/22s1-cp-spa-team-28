#pragma once

#include "Relationship.h"

/*
 * Class encapsulating Modifies relationship.
 */
class Modifies : Relationship<Reference, Reference> {
public:
    bool isValidRelationship(Reference, Reference) {
        return false;
    }
};