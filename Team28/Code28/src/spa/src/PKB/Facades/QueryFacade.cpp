#include <stdexcept>

#include "QueryFacade.h"

void QueryFacade::resetCache() {
    NextTTable *nextTTable = this->storage->getTable<NextTTable>();
    nextTTable->resetCache();
    AffectsTable *affectsTable = this->storage->getTable<AffectsTable>();
    affectsTable->resetCache();
    AffectsTTable *affectsTTable = this->storage->getTable<AffectsTTable>();
    affectsTTable->resetCache();
};

int QueryFacade::getTableSize(Designation desType) {
    Table *table = this->storage->getDesignationTable(desType);
    return table->getTableSize();
};

bool QueryFacade::validateWildcard(Reference leftRef, Reference rightRef,
                                   Solvable *sTable, Solvable *pTable) {
    return sTable->validate(leftRef, rightRef) ||
           pTable->validate(leftRef, rightRef);
}

std::vector<Value>
QueryFacade::concatSolveRightResults(std::vector<Solvable *> solvables,
                                     Reference leftRef,
                                     EntityName rightSynonym) {
    std::vector<Value> result = {};
    std::vector<Value> intermediateRes = {};
    for (Solvable *solvable : solvables) {
        intermediateRes = solvable->solveRight(leftRef, rightSynonym,
                                               this->storage->getStorageView());
        result.insert(result.end(), intermediateRes.begin(),
                      intermediateRes.end());
    }
    return result;
};

ReferenceType QueryFacade::getRefType(EntityName leftSynonym) {
    if (stmtRefSet.count(leftSynonym) == 1) {
        return ReferenceType::STMT_REF;
    }
    if (leftSynonym == EntityName::PROCEDURE) {
        return ReferenceType::ENT_REF;
    }
    return ReferenceType::WILDCARD;
}

QueryFacade::QueryFacade(Storage *storage) { this->storage = storage; }

std::vector<Statement *>
QueryFacade::getAllStatementsByType(StatementType type) {
    StatementsTable *statements = this->storage->getTable<StatementsTable>();
    if (type == StatementType::STMT) {
        return statements->getAll();
    }
    std::vector<int> statementTypeIndices =
        statements->getStatementsByType(type);
    std::vector<Statement *> results;
    for (int i : statementTypeIndices) {
        results.push_back(new Statement(i, type));
    }
    return results;
}

std::vector<std::string> QueryFacade::getAllEntities(Designation entityType) {
    if (namedEntitiesSet.count(entityType) == 0) {
        return std::vector<std::string>();
    }
    Table *entityTable = this->storage->getDesignationTable(entityType);
    return entityTable->getAllAsString();
}

bool QueryFacade::validate(RelationshipReference relType, Reference leftRef,
                           Reference rightRef) {
    if (leftRef.isASynonym() || rightRef.isASynonym()) {
        return false;
    }
    ReferenceType leftRefType = leftRef.getRefType();

    if (isWildcardedModifies(leftRefType, relType)) {
        std::vector<Solvable *> modifies = this->storage->getModifiesTables();
        return validateWildcard(leftRef, rightRef, modifies.at(0),
                                modifies.at(1));
    }

    if (isWildcardedUses(leftRefType, relType)) {
        std::vector<Solvable *> uses = this->storage->getUsesTables();
        return validateWildcard(leftRef, rightRef, uses.at(0), uses.at(1));
    }

    Solvable *table = this->storage->getRsTable(relType, leftRefType);
    return table->validate(leftRef, rightRef);
}

bool QueryFacade::validate(int stmtNo, std::string varName,
                           AssignExpression expr) {
    AssignmentsTable *table = this->storage->getTable<AssignmentsTable>();
    return table->validate(stmtNo, varName, expr);
}

bool QueryFacade::validate(Designation desType, int stmtNo,
                           std::string varName) {
    if (condPatternSet.count(desType) == 0) {
        return false;
    }
    UsesControlVarTable *conds = this->storage->getControlVarTable(desType);
    return conds->validate(stmtNo, varName);
}

std::vector<Value> QueryFacade::solveRight(RelationshipReference relType,
                                           Reference leftRef,
                                           EntityName rightSynonym) {
    if (leftRef.isASynonym()) {
        return std::vector<Value>();
    }
    ReferenceType leftRefType = leftRef.getRefType();

    if (isWildcardedModifies(leftRefType, relType)) {
        std::vector<Solvable *> modifies = this->storage->getModifiesTables();
        return concatSolveRightResults(modifies, leftRef, rightSynonym);
    }

    if (isWildcardedUses(leftRefType, relType)) {
        std::vector<Solvable *> uses = this->storage->getUsesTables();
        return concatSolveRightResults(uses, leftRef, rightSynonym);
    }

    Solvable *table = this->storage->getRsTable(relType, leftRefType);

    return table->solveRight(leftRef, rightSynonym,
                             this->storage->getStorageView());
}

std::vector<Value> QueryFacade::solveLeft(RelationshipReference relType,
                                          Reference rightRef,
                                          EntityName leftSynonym) {
    if (rightRef.isASynonym()) {
        return std::vector<Value>();
    }
    ReferenceType leftRef = this->getRefType(leftSynonym);
    if (isWildcardedUses(leftRef, relType) ||
        isWildcardedModifies(leftRef, relType)) {
        return std::vector<Value>();
    }

    Solvable *table = this->storage->getRsTable(relType, leftRef);
    return table->solveLeft(rightRef, leftSynonym,
                            this->storage->getStorageView());
}

std::vector<std::pair<Value, Value>>
QueryFacade::solveBoth(RelationshipReference relType, EntityName leftSynonym,
                       EntityName rightSynonym) {

    ReferenceType leftRef = this->getRefType(leftSynonym);
    if (isWildcardedUses(leftRef, relType) ||
        isWildcardedModifies(leftRef, relType)) {
        return std::vector<std::pair<Value, Value>>();
    }
    Solvable *table = this->storage->getRsTable(relType, leftRef);
    return table->solveBoth(leftSynonym, rightSynonym,
                            this->storage->getStorageView());
}

std::vector<Value> QueryFacade::getAssign(std::string varName,
                                          AssignExpression expression) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();
    if (expression.isExactExpression()) {
        return assignments->getAssignExact(varName, expression.getExpression());
    }
    return assignments->getAssign(varName, expression.getExpression());
};

std::vector<std::pair<Value, Value>>
QueryFacade::getAssignAndVar(AssignExpression expression) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();
    if (expression.isExactExpression()) {
        return assignments->getAssignAndVarExact(expression.getExpression());
    }
    return assignments->getAssignAndVar(expression.getExpression());
};

std::vector<Value> QueryFacade::getCond(Designation condType,
                                        std::string varName) {
    if (condPatternSet.count(condType) == 0) {
        return std::vector<Value>();
    }
    UsesControlVarTable *conds = this->storage->getControlVarTable(condType);
    return conds->getStmt(varName);
};

std::vector<std::pair<Value, Value>>
QueryFacade::getCondAndVar(Designation condType) {
    if (condPatternSet.count(condType) == 0) {
        return std::vector<std::pair<Value, Value>>();
    }
    UsesControlVarTable *conds = this->storage->getControlVarTable(condType);
    return conds->getStmtAndVar();
};

std::string QueryFacade::getSecondaryAttribute(int stmtNum) {
    StatementsTable *statements = this->storage->getTable<StatementsTable>();
    if (!statements->hasSecondaryAttribute(stmtNum)) {
        throw std::invalid_argument(STMT_NO_SECONDARY_ATTRIBUTE);
    }

    CallProcTable *callProc = this->storage->getTable<CallProcTable>();
    if (callProc->isLeftValueExist(stmtNum)) {
        // Check calls r/s first since call stmts can use and modify as well.
        return callProc->retrieveSingleRight(stmtNum);
    }

    UsesSTable *usesS = this->storage->getTable<UsesSTable>();
    if (usesS->isLeftValueExist(stmtNum)) {
        // Print only
        return usesS->retrieveSingleRight(stmtNum);
    }

    ModifiesSTable *modifiesS = this->storage->getTable<ModifiesSTable>();
    if (modifiesS->isLeftValueExist(stmtNum)) {
        // Read only
        return modifiesS->retrieveSingleRight(stmtNum);
    }

    return "";
};

std::vector<Value> QueryFacade::solveOneAttribute(Reference ref, Value value) {
    std::string v = value.getValue();
    EntityName entity = ref.getEntityName();
    Table *table = this->storage->getAttributesTable(entity, ref.getAttr());

    return table->getMatchingValue(v, entity);
}

std::vector<std::pair<Value, Value>>
QueryFacade::solveBothAttribute(Reference left, Reference right) {
    // TODO: find a method to inner join instead of cross product
    EntityName leftEnt = left.getEntityName();
    Table *leftTable =
        this->storage->getAttributesTable(leftEnt, left.getAttr());
    std::map<Value, std::vector<Value>> leftValuesMap =
        leftTable->getAllValues(leftEnt);

    EntityName rightEnt = right.getEntityName();
    Table *rightTable =
        this->storage->getAttributesTable(rightEnt, right.getAttr());
    std::map<Value, std::vector<Value>> rightValuesMap =
        rightTable->getAllValues(rightEnt);

    std::vector<std::pair<Value, Value>> result;
    for (auto const &[key, value] : leftValuesMap) {
        if (rightValuesMap.count(key) > 0) {
            this->addAllPairsInto(&result, &leftValuesMap[key],
                                  &rightValuesMap[key]);
        }
    }

    return result;
}

void QueryFacade::addAllPairsInto(std::vector<std::pair<Value, Value>> *result,
                                  std::vector<Value> *left,
                                  std::vector<Value> *right) {
    for (Value lValue : *left) {
        for (Value rValue : *right) {
            result->push_back(std::make_pair(lValue, rValue));
        }
    }
};

bool QueryFacade::isWildcardedUses(ReferenceType leftRef,
                                   RelationshipReference relType) {
    return leftRef == ReferenceType::WILDCARD &&
           relType == RelationshipReference::USES;
}

bool QueryFacade::isWildcardedModifies(ReferenceType leftRef,
                                       RelationshipReference relType) {
    return leftRef == ReferenceType::WILDCARD &&
           relType == RelationshipReference::MODIFIES;
}

std::vector<Value> QueryFacade::solveReflexive(RelationshipReference rsRef,
                                               EntityName stmtEntity) {
    if (stmtRefSet.count(stmtEntity) != 1) {
        return std::vector<Value>();
    }

    Reflexive *reflexive = this->storage->getReflexiveTable(rsRef);
    return reflexive->solveBothReflexive(stmtEntity,
                                         this->storage->getStorageView());
};

std::vector<Value> QueryFacade::getVar(int stmtNo, AssignExpression expr) {
    AssignmentsTable *table = this->storage->getTable<AssignmentsTable>();
    return table->getVar(stmtNo, expr);
}

std::vector<Value> QueryFacade::getVar(Designation desType, int stmtNo) {
    if (condPatternSet.count(desType) == 0) {
        return std::vector<Value>();
    }
    UsesControlVarTable *conds = this->storage->getControlVarTable(desType);
    return conds->getVar(stmtNo);
}
