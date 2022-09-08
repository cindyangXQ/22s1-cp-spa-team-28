#include "SuchThatClause.h"

SuchThatClause::SuchThatClause() {
	this->relationship = Relationship::EMPTY;
}
SuchThatClause::SuchThatClause(Relationship relationshipType, Reference refLeft, Reference refRight) {
	this->relationship = relationshipType;
	this->refLeft = refLeft;
	this->refRight = refRight;
}