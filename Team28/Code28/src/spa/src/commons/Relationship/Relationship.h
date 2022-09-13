#pragma once

#include "../Entity.h"
#include "../Reference.h"
#include "../../QPS/Error/SemanticError.h"
#include "../../QPS/Evaluator/ClauseResult.h"

/*
 * Enumerates the different design entity.
 */
enum class RelationshipReference {
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    USES,
    MODIFIES,
    EMPTY
};

/*
* Enumerates the different headers to filter by.
*/
enum class RelationshipHeader {
	CHECK_LEFT,
	CHECK_RIGHT
};

/*
* A class encapsulating Relationships in SIMPLE.
*/
template <typename Left, typename Right>
class Relationship {
public:
    /*
    * Explicit constructor for Relationship.
    */
    explicit Relationship(RelationshipReference relRef, Left left, Right right) {
        this->relRef = relRef;
        this->left = left;
        this->right = right;
    };

    // virtual bool isValidRelationship(Left, Right);

    Left getLeft() {
        return this->left;
    };

    Right getRight() {
        return this->right;
    };

    RelationshipReference getRelationshipReference() {
        return this->relRef;
    }

private:
    RelationshipReference relRef;
    Left left;
    Right right;
};
