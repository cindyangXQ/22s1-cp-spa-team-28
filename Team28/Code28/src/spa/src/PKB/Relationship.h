#pragma once

#include "Entity.h"

enum class RelationshipName {
	FOLLOWS,
	FOLLOWS_STAR,
	PARENT,
	PARENT_STAR,
	USES,
	MODIFIES
};

/*
* A class encapsulating Relationships in SIMPLE.
*/
template <typename Left, typename Right>
class Relationship : public Entity<RelationshipName> {
public:
	/*
	* Explicit constructor for Relationship.
	*/
	explicit Relationship(RelationshipName name);

	Left getLeft();

	Right getRight();

private:
	Left* left;
	Right* right;
};