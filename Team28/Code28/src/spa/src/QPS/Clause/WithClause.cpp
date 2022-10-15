#include "WithClause.h"

WithClause::WithClause() {}
WithClause::WithClause(Reference refLeft, Reference refRight) {
    this->refLeft = refLeft;
    this->refRight = refRight;
}
