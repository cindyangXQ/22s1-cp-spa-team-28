#include "SuchThatClause.h"

SuchThatClause::SuchThatClause() {
	this->relationshipType = RelationshipReference::Empty;
}
SuchThatClause::SuchThatClause(RelationshipReference relationshipType, Reference refLeft, Reference refRight) {
	this->relationshipType = relationshipType;
	this->refLeft = refLeft;
	this->refRight = refRight;
}