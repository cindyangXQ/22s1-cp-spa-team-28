#include "AffectsTTable.h"

void AffectsTTable::initAffectsT(StorageView *storage) {
    StatementsTable *statements = storage->getTable<StatementsTable>();
    AffectsTable *affects = storage->getTable<AffectsTable>();
    this->affects = affects;
    this->matrix = affects->getMatrix();
    this->totalLines = statements->getTableSize();
}

bool AffectsTTable::validate(Reference leftRef, Reference rightRef) {
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                std::pair curr = std::make_pair(left, right);
                if (this->matrix.at(curr) == Status::TRUE) {
                    return true;
                }
                if (this->matrix.at(curr) == Status::FALSE) {
                    return false;
                }
            }
        }
        return false;
    }
}
