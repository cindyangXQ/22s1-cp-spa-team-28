#include <stdexcept>

#include "QueryFacade.h"

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
    if (leftRef.isSynonym || rightRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return false;
    }

    if (relType == RelationshipReference::MODIFIES &&
        leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable *> modifies = this->storage->getModifiesTables();
        return validateWildcard(leftRef, rightRef, modifies.at(0),
                                modifies.at(1));
    }

    if (relType == RelationshipReference::USES &&
        leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable *> uses = this->storage->getUsesTables();
        return validateWildcard(leftRef, rightRef, uses.at(0), uses.at(1));
    }

    Solvable *table = this->storage->getRsTable(relType, leftRef.type);
    return table->validate(leftRef, rightRef);
}

std::vector<Value> QueryFacade::solveRight(RelationshipReference relType,
                                           Reference leftRef,
                                           EntityName rightSynonym) {
    if (leftRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return std::vector<Value>();
    }

    if (relType == RelationshipReference::MODIFIES &&
        leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable *> modifies = this->storage->getModifiesTables();
        return concatSolveRightResults(modifies, leftRef, rightSynonym);
    }

    if (relType == RelationshipReference::USES &&
        leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable *> uses = this->storage->getUsesTables();
        return concatSolveRightResults(uses, leftRef, rightSynonym);
    }

    Solvable *table = this->storage->getRsTable(relType, leftRef.type);

    return table->solveRight(leftRef, rightSynonym,
                             this->storage->getStorageView());
}

std::vector<Value> QueryFacade::solveLeft(RelationshipReference relType,
                                          Reference rightRef,
                                          EntityName leftSynonym) {
    if (rightRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return std::vector<Value>();
    }
    ReferenceType leftRef = this->getRefType(leftSynonym);
    if (leftRef == ReferenceType::WILDCARD &&
        (relType == RelationshipReference::USES ||
         relType == RelationshipReference::MODIFIES)) {
        // TODO: Throw error instead of return empty list if needed.
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
    if (leftRef == ReferenceType::WILDCARD &&
        (relType == RelationshipReference::USES ||
         relType == RelationshipReference::MODIFIES)) {
        // TODO: Throw error instead of return empty list if needed.
        return std::vector<std::pair<Value, Value>>();
    }
    Solvable *table = this->storage->getRsTable(relType, leftRef);
    return table->solveBoth(leftSynonym, rightSynonym,
                            this->storage->getStorageView());
}

std::vector<Value> QueryFacade::getAssign(std::string varName,
                                          std::string expression) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();
    return assignments->getAssign(varName, expression);
};

std::vector<Value> QueryFacade::getAssignExact(std::string varName,
                                               std::string expression) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();
    return assignments->getAssignExact(varName, expression);
};

std::vector<std::pair<Value, Value>>
QueryFacade::getAssignAndVar(std::string expression) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();
    return assignments->getAssignAndVar(expression);
};

std::vector<std::pair<Value, Value>>
QueryFacade::getAssignAndVarExact(std::string expression) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();
    return assignments->getAssignAndVarExact(expression);
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
        throw std::invalid_argument(
            "StmtNum does not refer to attributable statement");
    }
    UsesSTable *usesS = this->storage->getTable<UsesSTable>();
    if (usesS->isLeftValueExist(stmtNum)) {
        return usesS->retrieveSingleRight(stmtNum);
    }

    ModifiesSTable *modifiesS = this->storage->getTable<ModifiesSTable>();
    if (modifiesS->isLeftValueExist(stmtNum)) {
        return modifiesS->retrieveSingleRight(stmtNum);
    }

    CallProcTable *callProc = this->storage->getTable<CallProcTable>();
    return callProc->retrieveSingleRight(stmtNum);
};
