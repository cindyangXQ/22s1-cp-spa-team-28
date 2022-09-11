#include "SuchThatClause.h"

SuchThatClause::SuchThatClause() {
	this->relationship = RelationshipReference::EMPTY;
}
SuchThatClause::SuchThatClause(RelationshipReference relationshipReference, Reference refLeft, Reference refRight) {
	this->relationship = relationshipReference;
	this->refLeft = refLeft;
	this->refRight = refRight;
}