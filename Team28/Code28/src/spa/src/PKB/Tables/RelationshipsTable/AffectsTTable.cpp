#include "AffectsTTable.h"

void AffectsTTable::initAffectsT(StorageView *storage) {
    AffectsTable *affects = storage->getTable<AffectsTable>();
    StatementsTable *statements = storage->getTable<StatementsTable>();
    this->matrix = affects->eagerGetMatrix();
    this->totalLines = statements->getTableSize();
}

void AffectsTTable::populateAffectsT() {
    std::map<std::pair<int, int>, bool> matrix = computeClosure();
    for (const auto &elem : matrix) {
        if (elem.second) {
            Relationship<int, int> affectsTRS =
                Relationship(RelationshipReference::AFFECTS_T, elem.first.first,
                             elem.first.second);
            TableValue *toStore = static_cast<TableValue *>(&affectsTRS);
            this->store(toStore);
        }
    }
}

std::map<std::pair<int, int>, bool> AffectsTTable::computeClosure() {
    // TODO: Clean up/change method entirely
    std::vector<std::pair<int, int>> validPos;
    std::map<std::pair<int, int>, bool> intermediate;
    std::map<std::pair<int, int>, bool> final;

    for (int i = 1; i <= this->totalLines; i++) {
        for (int j = 1; j <= this->totalLines; j++) {
            std::pair<int, int> curr = std::make_pair(i, j);
            intermediate[curr] = false;
        }
    }
    for (const auto &elem : this->matrix) {
        intermediate[elem.first] = elem.second;
        validPos.push_back(elem.first);
    }
    for (int k = 1; k <= this->totalLines; k++) {
        for (int i = 1; i <= this->totalLines; i++) {
            for (int j = 1; j <= this->totalLines; j++) {
                std::pair<int, int> curr = std::make_pair(i, j);
                std::pair<int, int> left = std::make_pair(i, k);
                std::pair<int, int> right = std::make_pair(k, j);
                intermediate[curr] =
                    intermediate[curr] ||
                    (intermediate[left] && intermediate[right]);
            }
        }
    }
    for (std::pair pair : validPos) {
        final[pair] = intermediate[pair];
    }
    std::cout << "Computing Affects* closure..." << std::endl;
    for (const auto &p : final) {
        std::cout << "(" << p.first.first << "," << p.first.second << ") "
                  << p.second << std::endl;
    }
    return final;
}
