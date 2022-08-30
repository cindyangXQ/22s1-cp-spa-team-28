#include "SuchThatClause.h"

SuchThatClause::SuchThatClause() {
	this->relationshipType = RelRef::Empty;
}
SuchThatClause::SuchThatClause(RelRef relationshipType, Reference refLeft, Reference ref_2) {
	this->relationshipType = relationshipType;
	this->refLeft = refLeft;
	this->refRight = refRight;
}