#pragma once

#include "Relationship.h"

/*
 * Class encapsulating Follows relationship.
 */
class Follows : Relationship<Reference, Reference> {
public:
    bool isValidRelationship(Reference, Reference) {

    }
};