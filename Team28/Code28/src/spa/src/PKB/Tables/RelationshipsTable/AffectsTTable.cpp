#include "AffectsTTable.h"

void AffectsTTable::initAffectsT(StorageView *storage) {
    StatementsTable *statements = storage->getTable<StatementsTable>();
    AffectsTable *affects = storage->getTable<AffectsTable>();
    this->affects = affects;
    this->totalLines = statements->getTableSize();
    this->assignments =
        statements->getStatementsSetByType(StatementType::ASSIGN);
}

bool AffectsTTable::validate(Reference leftRef, Reference rightRef) {
    std::map<std::pair<int, int>, Status> matrix = this->affects->getMatrix();
    if (leftRef.isWildcard() && rightRef.isWildcard()) {
        for (int left : this->assignments) {
            for (int right : this->assignments) {
                std::pair curr = std::make_pair(left, right);
                if (matrix.at(curr) == Status::TRUE) {
                    return true;
                }
                if (matrix.at(curr) == Status::FALSE) {
                    return false;
                }
            }
        }
        return false;
    }
}

std::vector<Value> AffectsTTable::solveRight(Reference leftRef,
                                             EntityName rightSynonym,
                                             StorageView *storage);

std::vector<Value> AffectsTTable::solveLeft(Reference rightRef,
                                            EntityName leftSynonym,
                                            StorageView *storage);

std::vector<std::pair<Value, Value>>
AffectsTTable::solveBoth(EntityName leftSynonym, EntityName rightSynonym,
                         StorageView *storage);

std::vector<Value> AffectsTTable::solveBothReflexive(EntityName synonym,
                                                     StorageView *storage);
