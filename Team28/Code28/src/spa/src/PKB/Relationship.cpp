#include "Relationship.h"

Relationship::Relationship(RelationshipName name) : Entity<RelationshipName>(name) {}

template<typename Left, typename Right>
Left Relationship<Left, Right>::getLeft() {
	return this->left;
};

template<typename Left, typename Right>
Right Relationship<Left, Right>::getRight() {
	return this->right;
};
