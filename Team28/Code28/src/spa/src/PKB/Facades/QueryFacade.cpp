#include "QueryFacade.h"

QueryFacade::QueryFacade(Storage *storage) { this->storage = storage; }

std::vector<Statement *> QueryFacade::getAllStatements() {
    StatementsTable *statements =
        this->storage->getTable<StatementsTable>(TableName::STATEMENTS);

    return statements->getAll();
}

std::vector<Statement *>
QueryFacade::getAllStatementsByType(StatementType type) {
    StatementsTable *statements =
        this->storage->getTable<StatementsTable>(TableName::STATEMENTS);
    std::vector<int> statementTypeIndices =
        statements->getStatementsByType(type);
    std::vector<Statement *> results;
    for (int i : statementTypeIndices) {
        results.push_back(new Statement(i, type));
    }
    return results;
}

Statement *QueryFacade::getStatementByLineNo(const int &lineNo) {
    StatementsTable *statements =
        this->storage->getTable<StatementsTable>(TableName::STATEMENTS);

    return statements->retrieve(lineNo);
}

std::vector<std::string> QueryFacade::getAllVariables() {
    VariablesTable *variables =
        this->storage->getTable<VariablesTable>(TableName::VARIABLES);
    std::unordered_set names = variables->getAll();
    std::vector<std::string> result(names.begin(), names.end());

    return result;
}

Variable *QueryFacade::getVariableByName(const std::string &name) {
    VariablesTable *variables =
        this->storage->getTable<VariablesTable>(TableName::VARIABLES);

    return variables->retrieve(name);
}

std::vector<std::string> QueryFacade::getAllConstants() {
    ConstantsTable *constants =
        this->storage->getTable<ConstantsTable>(TableName::CONSTANTS);
    std::unordered_set names = constants->getAll();
    std::vector<std::string> result(names.begin(), names.end());

    return result;
}

Constant *QueryFacade::getConstantByName(const std::string &name) {
    ConstantsTable *constants =
        this->storage->getTable<ConstantsTable>(TableName::CONSTANTS);

    return constants->retrieve(name);
}

std::vector<std::string> QueryFacade::getAllProcedures() {
    ProceduresTable *procedures =
        this->storage->getTable<ProceduresTable>(TableName::PROCEDURES);
    std::unordered_set names = procedures->getAll();
    std::vector<std::string> result(names.begin(), names.end());

    return result;
}

Procedure *QueryFacade::getProcedureByName(const std::string &name) {
    ProceduresTable *procedures =
        this->storage->getTable<ProceduresTable>(TableName::PROCEDURES);

    return procedures->retrieve(name);
}

bool QueryFacade::validate(RelationshipReference relType, Reference leftRef,
                           Reference rightRef) {
    if (leftRef.isSynonym || rightRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return false;
    }
    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        FollowsTable *follows =
            this->storage->getTable<FollowsTable>(TableName::FOLLOWS);
        return follows->validate(leftRef, rightRef);
    }
    case RelationshipReference::FOLLOWS_T: {
        FollowsTTable *followsT =
            this->storage->getTable<FollowsTTable>(TableName::FOLLOWS_T);
        return followsT->validate(leftRef, rightRef);
    }
    case RelationshipReference::PARENT: {
        ParentTable *parent =
            this->storage->getTable<ParentTable>(TableName::PARENT);
        return parent->validate(leftRef, rightRef);
    }
    case RelationshipReference::PARENT_T: {
        ParentTTable *parentT =
            this->storage->getTable<ParentTTable>(TableName::PARENT_T);
        return parentT->validate(leftRef, rightRef);
    }
    case RelationshipReference::MODIFIES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            ModifiesSTable *modifiesS =
                this->storage->getTable<ModifiesSTable>(TableName::MODIFIES_S);
            return modifiesS->validate(leftRef, rightRef);
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            ModifiesPTable *modifiesP =
                this->storage->getTable<ModifiesPTable>(TableName::MODIFIES_P);
            return modifiesP->validate(leftRef, rightRef);
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            ModifiesSTable *modifiesS =
                this->storage->getTable<ModifiesSTable>(TableName::MODIFIES_S);
            ModifiesPTable *modifiesP =
                this->storage->getTable<ModifiesPTable>(TableName::MODIFIES_P);
            return modifiesS->validate(leftRef, rightRef) ||
                   modifiesP->validate(leftRef, rightRef);
        }
        break;
    }
    case RelationshipReference::USES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            UsesSTable *usesS =
                this->storage->getTable<UsesSTable>(TableName::USES_S);
            return usesS->validate(leftRef, rightRef);
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            UsesPTable *usesP =
                this->storage->getTable<UsesPTable>(TableName::USES_P);
            return usesP->validate(leftRef, rightRef);
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            UsesSTable *usesS =
                this->storage->getTable<UsesSTable>(TableName::USES_S);
            UsesPTable *usesP =
                this->storage->getTable<UsesPTable>(TableName::USES_P);
            return usesS->validate(leftRef, rightRef) ||
                   usesP->validate(leftRef, rightRef);
        }
        break;
    }
    case RelationshipReference::CALLS: {
        CallsTable *calls =
            this->storage->getTable<CallsTable>(TableName::CALLS);
        return calls->validate(leftRef, rightRef);
    }
    case RelationshipReference::CALLS_T: {
        CallsTTable *callsT =
            this->storage->getTable<CallsTTable>(TableName::CALLS_T);
        return callsT->validate(leftRef, rightRef);
    }
    default: {
        // TODO: throw error instead of return false
        return false;
    }
    }
}

std::vector<Value> QueryFacade::solveRight(RelationshipReference relType,
                                           Reference leftRef,
                                           EntityName rightSynonym) {
    if (leftRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return std::vector<Value>();
    }
    StatementsTable *statements =
        this->storage->getTable<StatementsTable>(TableName::STATEMENTS);
    VariablesTable *variables =
        this->storage->getTable<VariablesTable>(TableName::VARIABLES);
    ProceduresTable *procedures =
        this->storage->getTable<ProceduresTable>(TableName::PROCEDURES);

    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        FollowsTable *follows =
            this->storage->getTable<FollowsTable>(TableName::FOLLOWS);
        return follows->solveRight(leftRef, rightSynonym, statements);
    }
    case RelationshipReference::FOLLOWS_T: {
        FollowsTTable *followsT =
            this->storage->getTable<FollowsTTable>(TableName::FOLLOWS_T);
        return followsT->solveRight(leftRef, rightSynonym, statements);
    }
    case RelationshipReference::PARENT: {
        ParentTable *parent =
            this->storage->getTable<ParentTable>(TableName::PARENT);
        return parent->solveRight(leftRef, rightSynonym, statements);
    }
    case RelationshipReference::PARENT_T: {
        ParentTTable *parentT =
            this->storage->getTable<ParentTTable>(TableName::PARENT_T);
        return parentT->solveRight(leftRef, rightSynonym, statements);
    }
    case RelationshipReference::MODIFIES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            ModifiesSTable *modifiesS =
                this->storage->getTable<ModifiesSTable>(TableName::MODIFIES_S);
            return modifiesS->solveRight(leftRef, rightSynonym, variables);
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            ModifiesPTable *modifiesP =
                this->storage->getTable<ModifiesPTable>(TableName::MODIFIES_P);
            return modifiesP->solveRight(leftRef, rightSynonym, variables);
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            ModifiesSTable *modifiesS =
                this->storage->getTable<ModifiesSTable>(TableName::MODIFIES_S);
            ModifiesPTable *modifiesP =
                this->storage->getTable<ModifiesPTable>(TableName::MODIFIES_P);
            std::vector<Value> stmtRes =
                modifiesS->solveRight(leftRef, rightSynonym, variables);
            std::vector<Value> procRes =
                modifiesP->solveRight(leftRef, rightSynonym, variables);
            std::vector<Value> result(stmtRes);
            result.insert(result.end(), procRes.begin(), procRes.end());
            return result;
        }
        break;
    }
    case RelationshipReference::USES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            UsesSTable *usesS =
                this->storage->getTable<UsesSTable>(TableName::USES_S);
            return usesS->solveRight(leftRef, rightSynonym, variables);
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            UsesPTable *usesP =
                this->storage->getTable<UsesPTable>(TableName::USES_P);
            return usesP->solveRight(leftRef, rightSynonym, variables);
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            UsesSTable *usesS =
                this->storage->getTable<UsesSTable>(TableName::USES_S);
            UsesPTable *usesP =
                this->storage->getTable<UsesPTable>(TableName::USES_P);
            std::vector<Value> stmtRes =
                usesS->solveRight(leftRef, rightSynonym, variables);
            std::vector<Value> procRes =
                usesP->solveRight(leftRef, rightSynonym, variables);
            std::vector<Value> result(stmtRes);
            result.insert(result.end(), procRes.begin(), procRes.end());
            return result;
        }
        break;
    }
    case RelationshipReference::CALLS: {
        CallsTable *calls =
            this->storage->getTable<CallsTable>(TableName::CALLS);
        return calls->solveRight(leftRef, rightSynonym, procedures);
    }
    case RelationshipReference::CALLS_T: {
        CallsTTable *callsT =
            this->storage->getTable<CallsTTable>(TableName::CALLS_T);
        return callsT->solveRight(leftRef, rightSynonym, procedures);
    }
    default: {
        // TODO: throw error instead of return false
        return std::vector<Value>();
    }
    }
}

std::vector<Value> QueryFacade::solveLeft(RelationshipReference relType,
                                          Reference rightRef,
                                          EntityName leftSynonym) {
    if (rightRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return std::vector<Value>();
    }
    StatementsTable *statements =
        this->storage->getTable<StatementsTable>(TableName::STATEMENTS);
    ProceduresTable *procedures =
        this->storage->getTable<ProceduresTable>(TableName::PROCEDURES);

    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        FollowsTable *follows =
            this->storage->getTable<FollowsTable>(TableName::FOLLOWS);
        return follows->solveLeft(rightRef, leftSynonym, statements);
    }
    case RelationshipReference::FOLLOWS_T: {
        FollowsTTable *followsT =
            this->storage->getTable<FollowsTTable>(TableName::FOLLOWS_T);
        return followsT->solveLeft(rightRef, leftSynonym, statements);
    }
    case RelationshipReference::PARENT: {
        ParentTable *parent =
            this->storage->getTable<ParentTable>(TableName::PARENT);
        return parent->solveLeft(rightRef, leftSynonym, statements);
    }
    case RelationshipReference::PARENT_T: {
        ParentTTable *parentT =
            this->storage->getTable<ParentTTable>(TableName::PARENT_T);
        return parentT->solveLeft(rightRef, leftSynonym, statements);
    }
    case RelationshipReference::MODIFIES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            ModifiesSTable *modifiesS =
                this->storage->getTable<ModifiesSTable>(TableName::MODIFIES_S);
            return modifiesS->solveLeft(rightRef, leftSynonym, statements);
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            ModifiesPTable *modifiesP =
                this->storage->getTable<ModifiesPTable>(TableName::MODIFIES_P);
            return modifiesP->solveLeft(rightRef, leftSynonym, procedures);
        }
        // TODO: throw error instead of returning empty list
        return std::vector<Value>();
    }
    case RelationshipReference::USES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            UsesSTable *usesS =
                this->storage->getTable<UsesSTable>(TableName::USES_S);
            return usesS->solveLeft(rightRef, leftSynonym, statements);
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            UsesPTable *usesP =
                this->storage->getTable<UsesPTable>(TableName::USES_P);
            return usesP->solveLeft(rightRef, leftSynonym, procedures);
        }
        // TODO: throw error instead of returning empty list
        return std::vector<Value>();
    }
    case RelationshipReference::CALLS: {
        CallsTable *calls =
            this->storage->getTable<CallsTable>(TableName::CALLS);
        return calls->solveLeft(rightRef, leftSynonym, procedures);
    }
    case RelationshipReference::CALLS_T: {
        CallsTTable *callsT =
            this->storage->getTable<CallsTTable>(TableName::CALLS_T);
        return callsT->solveLeft(rightRef, leftSynonym, procedures);
    }
    default: {
        // TODO: throw error instead of return false
        return std::vector<Value>();
    }
    }
}

std::vector<std::pair<Value, Value>>
QueryFacade::solveBoth(RelationshipReference relType, EntityName leftSynonym,
                       EntityName rightSynonym) {
    StatementsTable *statements =
        this->storage->getTable<StatementsTable>(TableName::STATEMENTS);
    ProceduresTable *procedures =
        this->storage->getTable<ProceduresTable>(TableName::PROCEDURES);
    VariablesTable *variables =
        this->storage->getTable<VariablesTable>(TableName::VARIABLES);

    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        FollowsTable *follows =
            this->storage->getTable<FollowsTable>(TableName::FOLLOWS);
        return follows->solveBoth(leftSynonym, rightSynonym, statements);
    }
    case RelationshipReference::FOLLOWS_T: {
        FollowsTTable *followsT =
            this->storage->getTable<FollowsTTable>(TableName::FOLLOWS_T);
        return followsT->solveBoth(leftSynonym, rightSynonym, statements);
    }
    case RelationshipReference::PARENT: {
        ParentTable *parent =
            this->storage->getTable<ParentTable>(TableName::PARENT);
        return parent->solveBoth(leftSynonym, rightSynonym, statements);
    }
    case RelationshipReference::PARENT_T: {
        ParentTTable *parentT =
            this->storage->getTable<ParentTTable>(TableName::PARENT_T);
        return parentT->solveBoth(leftSynonym, rightSynonym, statements);
    }
    case RelationshipReference::MODIFIES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            ModifiesSTable *modifiesS =
                this->storage->getTable<ModifiesSTable>(TableName::MODIFIES_S);
            return modifiesS->solveBoth(leftSynonym, rightSynonym, statements,
                                        variables);
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            ModifiesPTable *modifiesP =
                this->storage->getTable<ModifiesPTable>(TableName::MODIFIES_P);
            return modifiesP->solveBoth(leftSynonym, rightSynonym, procedures,
                                        variables);
        }
        // TODO: throw error instead of returning empty list
        return std::vector<std::pair<Value, Value>>();
    }
    case RelationshipReference::USES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            UsesSTable *usesS =
                this->storage->getTable<UsesSTable>(TableName::USES_S);
            return usesS->solveBoth(leftSynonym, rightSynonym, statements,
                                    variables);
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            UsesPTable *usesP =
                this->storage->getTable<UsesPTable>(TableName::USES_P);
            return usesP->solveBoth(leftSynonym, rightSynonym, procedures,
                                    variables);
        }
        // TODO: throw error instead of returning empty list
        return std::vector<std::pair<Value, Value>>();
    }
    case RelationshipReference::CALLS: {
        CallsTable *calls =
            this->storage->getTable<CallsTable>(TableName::CALLS);
        return calls->solveBoth(leftSynonym, rightSynonym, procedures);
    }
    case RelationshipReference::CALLS_T: {
        CallsTTable *callsT =
            this->storage->getTable<CallsTTable>(TableName::CALLS_T);
        return callsT->solveBoth(leftSynonym, rightSynonym, procedures);
    }
    default: {
        // TODO: throw error instead of return false
        return std::vector<std::pair<Value, Value>>();
    }
    }
}

std::vector<Value> QueryFacade::getAssign(std::string varName,
                                          std::string expression,
                                          bool isExactExpr) {
    AssignmentsTable *assignments =
        this->storage->getTable<AssignmentsTable>(TableName::ASSIGNMENTS);

    if (isExactExpr) {
        return assignments->getAssignExact(varName, expression);
    }
    return assignments->getAssign(varName, expression);
};

std::vector<std::pair<Value, Value>>
QueryFacade::getAssignAndVar(std::string expression, bool isExactExpr) {
    AssignmentsTable *assignments =
        this->storage->getTable<AssignmentsTable>(TableName::ASSIGNMENTS);

    if (isExactExpr) {
        return assignments->getAssignAndVarExact(expression);
    }
    return assignments->getAssignAndVar(expression);
};

std::vector<Value> QueryFacade::getWhile(std::string varName) {
    WhileControlVarTable *whiles =
        this->storage->getTable<WhileControlVarTable>(TableName::W_CONTROL);

    return whiles->getStmt(varName);
}

std::vector<std::pair<Value, Value>> QueryFacade::getWhileAndVar() {
    WhileControlVarTable *whiles =
        this->storage->getTable<WhileControlVarTable>(TableName::W_CONTROL);

    return whiles->getStmtAndVar();
};

std::vector<Value> QueryFacade::getIf(std::string varName) {
    IfControlVarTable *ifs =
        this->storage->getTable<IfControlVarTable>(TableName::I_CONTROL);

    return ifs->getStmt(varName);
}

std::vector<std::pair<Value, Value>> QueryFacade::getIfAndVar() {
    IfControlVarTable *ifs =
        this->storage->getTable<IfControlVarTable>(TableName::I_CONTROL);

    return ifs->getStmtAndVar();
};
