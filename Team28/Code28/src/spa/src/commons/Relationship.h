#pragma once

#include "Entity.h"
#include "Reference.h"

/*
 * Enumerates the different design entity.
 */
enum class RelationshipType {
    FOLLOWS,
    FOLLOWS_STAR,
    PARENT,
    PARENT_STAR,
    USES,
    MODIFIES,
    CALLS,
    CALLS_STAR,
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
	explicit Relationship(RelationshipReference type, Left left, Right right) {
		this->relType = type;
		this->left = left;
		this->right = right;
	};

	Left getLeft() {
		return this->left;
	};

	Right getRight() {
		return this->right;
	};

	RelationshipReference getRelationshipType() {
		return this->relType;
	}
	
private:
	RelationshipReference relType;
	Left left;
	Right right;
};
