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
* A class encapsulating Relationships in SIMPLE.
*/
template <typename Left, typename Right>
class Relationship : public Entity<RelationshipReference> {
public:
	/*
	* Explicit constructor for Relationship.
	*/
	explicit Relationship(RelationshipReference name, Left left, Right right) : Entity<RelationshipReference>(name) {
		this->left = left;
		this->right = right;
	};

	Left getLeft() {
		return this->left;
	};

	Right getRight() {
		return this->right;
	};

private:
	Left left;
	Right right;
};
