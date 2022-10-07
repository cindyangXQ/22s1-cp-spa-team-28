#include "QueryFacade.h"

bool QueryFacade::validateWildcard(Reference leftRef, Reference rightRef,
                                   Solvable *sTable, Solvable *pTable) {
    return sTable->validate(leftRef, rightRef) ||
           pTable->validate(leftRef, rightRef);
}


std::vector<Value> 
QueryFacade::concatSolveRightResults(std::vector<Solvable*> solvables,
    Reference leftRef, EntityName rightSynonym) {
    std::vector<Value> result = {};
    std::vector<Value> intermediateRes = {};
    for (Solvable* solvable : solvables) {
        intermediateRes = solvable->solveRight(
            leftRef, rightSynonym, this->storage->getStorageView());
        result.insert(result.end(), intermediateRes.begin(), intermediateRes.end());
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

std::vector<Statement *> QueryFacade::getAllStatements() {
    StatementsTable *statements = this->storage->getTable<StatementsTable>();

    return statements->getAll();
}

std::vector<Statement *>
QueryFacade::getAllStatementsByType(StatementType type) {
    StatementsTable *statements = this->storage->getTable<StatementsTable>();
    std::vector<int> statementTypeIndices =
        statements->getStatementsByType(type);
    std::vector<Statement *> results;
    for (int i : statementTypeIndices) {
        results.push_back(new Statement(i, type));
    }
    return results;
}

Statement *QueryFacade::getStatementByLineNo(const int &lineNo) {
    StatementsTable *statements = this->storage->getTable<StatementsTable>();

    return statements->retrieve(lineNo);
}

std::vector<std::string> QueryFacade::getAllVariables() {
    VariablesTable *variables = this->storage->getTable<VariablesTable>();
    std::unordered_set names = variables->getAll();
    std::vector<std::string> result(names.begin(), names.end());

    return result;
}

Variable *QueryFacade::getVariableByName(const std::string &name) {
    VariablesTable *variables = this->storage->getTable<VariablesTable>();

    return variables->retrieve(name);
}

std::vector<std::string> QueryFacade::getAllConstants() {
    ConstantsTable *constants = this->storage->getTable<ConstantsTable>();
    std::unordered_set names = constants->getAll();
    std::vector<std::string> result(names.begin(), names.end());

    return result;
}

Constant *QueryFacade::getConstantByName(const std::string &name) {
    ConstantsTable *constants = this->storage->getTable<ConstantsTable>();

    return constants->retrieve(name);
}

std::vector<std::string> QueryFacade::getAllProcedures() {
    ProceduresTable *procedures = this->storage->getTable<ProceduresTable>();
    std::unordered_set names = procedures->getAll();
    std::vector<std::string> result(names.begin(), names.end());

    return result;
}

Procedure *QueryFacade::getProcedureByName(const std::string &name) {
    ProceduresTable *procedures = this->storage->getTable<ProceduresTable>();

    return procedures->retrieve(name);
}

bool QueryFacade::validate(RelationshipReference relType, Reference leftRef,
                           Reference rightRef) {
    if (leftRef.isSynonym || rightRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return false;
    }

    if (relType == RelationshipReference::MODIFIES && leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable*> modifies = this->storage->getModifiesTables();
        return validateWildcard(leftRef, rightRef, modifies.at(0), modifies.at(1));
    }

    if (relType == RelationshipReference::USES && leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable*> uses = this->storage->getUsesTables();
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

    if (relType == RelationshipReference::MODIFIES && leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable*> modifies = this->storage->getModifiesTables();
        return concatSolveRightResults(modifies, leftRef, rightSynonym);
    }

    if (relType == RelationshipReference::USES && leftRef.type == ReferenceType::WILDCARD) {
        std::vector<Solvable*> uses = this->storage->getUsesTables();
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
    if (leftRef == ReferenceType::WILDCARD && (relType == RelationshipReference::USES ||
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
    if (leftRef == ReferenceType::WILDCARD && (relType == RelationshipReference::USES ||
        relType == RelationshipReference::MODIFIES)) {
        // TODO: Throw error instead of return empty list if needed.
        return std::vector<std::pair<Value, Value>>();
    }
    Solvable *table = this->storage->getRsTable(relType, leftRef);
    return table->solveBoth(leftSynonym, rightSynonym,
                            this->storage->getStorageView());
}

std::vector<Value> QueryFacade::getAssign(std::string varName,
                                          std::string expression,
                                          bool isExactExpr) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();

    if (isExactExpr) {
        return assignments->getAssignExact(varName, expression);
    }
    return assignments->getAssign(varName, expression);
};

std::vector<std::pair<Value, Value>>
QueryFacade::getAssignAndVar(std::string expression, bool isExactExpr) {
    AssignmentsTable *assignments = this->storage->getTable<AssignmentsTable>();

    if (isExactExpr) {
        return assignments->getAssignAndVarExact(expression);
    }
    return assignments->getAssignAndVar(expression);
};

std::vector<Value> QueryFacade::getWhile(std::string varName) {
    WhileControlVarTable *whiles =
        this->storage->getTable<WhileControlVarTable>();

    return whiles->getStmt(varName);
}

std::vector<std::pair<Value, Value>> QueryFacade::getWhileAndVar() {
    WhileControlVarTable *whiles =
        this->storage->getTable<WhileControlVarTable>();

    return whiles->getStmtAndVar();
};

std::vector<Value> QueryFacade::getIf(std::string varName) {
    IfControlVarTable *ifs = this->storage->getTable<IfControlVarTable>();

    return ifs->getStmt(varName);
}

std::vector<std::pair<Value, Value>> QueryFacade::getIfAndVar() {
    IfControlVarTable *ifs = this->storage->getTable<IfControlVarTable>();

    return ifs->getStmtAndVar();
};
