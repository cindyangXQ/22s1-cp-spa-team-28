#include "AffectsBaseTable.h"

int AffectsBaseTable::getTableSize() { return INT_MAX; };

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

bool AffectsBaseTable::validateHelper(
    Reference leftRef, Reference rightRef,
    const std::function<bool()> &verifyDoubleWildcards,
    const std::function<bool(int, Position)> &verifySingleWildcard,
    const std::function<bool(int, int)> &checkRs) {
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        return verifyDoubleWildcards();
    }
    if (leftRef.isWildcard()) {
        int right = convertToType<int>(rightRef.getValueString());
        return verifySingleWildcard(right, Position::RIGHT);
    }
    if (rightRef.isWildcard()) {
        int left = convertToType<int>(leftRef.getValueString());
        return verifySingleWildcard(left, Position::LEFT);
    }
    int left = convertToType<int>(leftRef.getValueString());
    int right = convertToType<int>(rightRef.getValueString());
    if (!areAssignments(left, right)) {
        return false;
    }
    return checkRs(left, right);
};
