#include "SuchThatClause.h"

SuchThatClause::SuchThatClause() {
	this->relationshipType = RelRef::EMPTY;
}
SuchThatClause::SuchThatClause(RelRef relationshipType, Reference ref_1, Reference ref_2) {
	this->relationshipType = relationshipType;
	this->ref_1 = ref_1;
	this->ref_2 = ref_2;
}