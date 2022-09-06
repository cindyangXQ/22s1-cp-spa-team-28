#pragma once

#include "Entity.h"
#include "Reference.h"

/*
* A class encapsulating Relationships in SIMPLE.
*/
template <typename Left, typename Right>
class Relationship : public Entity<RelationshipReference> {
public:
	/*
	* Explicit constructor for Relationship.
	*/
	explicit Relationship(RelationshipReference name) : Entity<RelationshipReference>(name) {};

	Left getLeft() {
		return this->left;
	};

	Right getRight() {
		return this->right;
	};

private:
	Left* left;
	Right* right;
};