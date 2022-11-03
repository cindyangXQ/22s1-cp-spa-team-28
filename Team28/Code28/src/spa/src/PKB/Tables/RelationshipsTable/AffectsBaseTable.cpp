#include "AffectsBaseTable.h"

bool AffectsBaseTable::isAssignment(int stmt) {
    return this->assignments.count(stmt) > 0;
};

bool AffectsBaseTable::areAssignments(int left, int right) {
    return (this->assignments.count(left) > 0) &&
           (this->assignments.count(right) > 0);
};

bool AffectsBaseTable::isAssignmentEntity(EntityName entity) {
    return (entity == EntityName::ASSIGN) || (entity == EntityName::STMT);
};

int AffectsBaseTable::chooseStmt(int left, int right, Position pos) {
    return pos == Position::LEFT ? left : right;
};