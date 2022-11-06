#include "WithOptimizer.h"

void WithOptimizer::optimize(SolvableQuery *solvableQ) {

    std::vector<QueryClause *> clauses = solvableQ->getQueryClause();
    std::vector<QueryClause *> withClauses = solvableQ->getWithClause();
    std::vector<QueryClause *> newWithClauses;
    SelectClause selectClause = solvableQ->getSelectClause();
    std::vector<Reference> selectedRefs = selectClause.getRefs();
    std::unordered_set<std::string> selectedSynName;

    for (Reference ref : selectedRefs) {
        selectedSynName.insert(ref.getSynonymName());
    }
    for (int i = 0; i < withClauses.size(); i++) {
        WithClause *curr = dynamic_cast<WithClause *>(withClauses[i]);
        Reference left = curr->getRefLeft();
        Reference right = curr->getRefRight();

        Reference r;
        Reference v;
        if (left.getRefType() == ReferenceType::ATTR_REF &&
            right.getRefType() != ReferenceType::ATTR_REF) {
            if (!performReplacement(left, right, &clauses, &withClauses, i,
                                    selectedSynName)) {
                newWithClauses.push_back(curr);
            }
        } else if (left.getRefType() != ReferenceType::ATTR_REF &&
                   right.getRefType() == ReferenceType::ATTR_REF) {
            if (!performReplacement(right, left, &clauses, &withClauses, i,
                                    selectedSynName)) {
                newWithClauses.push_back(curr);
            }
        } else {
            newWithClauses.push_back(curr);
        }
    }
    solvableQ->replaceWithClause(newWithClauses);
}
bool WithOptimizer::performReplacement(
    Reference synRef, Reference valRef, std::vector<QueryClause *> *clauses,
    std::vector<QueryClause *> *withClauses, int i,
    std::unordered_set<std::string> selectedSynName) {
    if (synRef.isSecondaryAttribute()) {
        return false;
    } else if (selectedSynName.count(synRef.getSynonymName()) == 0) {
        bool replaced = false;
        for (int j = 0; j < clauses->size(); j++) {
            if ((*clauses)[j]->replace(synRef, valRef)) {
                replaced = true;
            }
        }
        for (int k = i + 1; k < withClauses->size(); k++) {
            if ((*withClauses)[k]->replace(synRef, valRef)) {
                replaced = true;
            }
        }
        return replaced;
    } else {
        return false;
    }
}