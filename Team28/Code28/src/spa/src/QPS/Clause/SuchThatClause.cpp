#include "SuchThatClause.h"

RelationshipReference SuchThatClause::getRelationship() {
    return this->relationship;
}
Reference SuchThatClause::getRefLeft() { return this->refLeft; }
Reference SuchThatClause::getRefRight() { return this->refRight; }