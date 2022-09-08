#include "SuchThatClause.h"

SuchThatClause::SuchThatClause() {
	this->relationship = RelationshipType::EMPTY;
}
SuchThatClause::SuchThatClause(RelationshipType relationshipType, Reference refLeft, Reference refRight) {
	this->relationship = relationshipType;
	this->refLeft = refLeft;
	this->refRight = refRight;
}