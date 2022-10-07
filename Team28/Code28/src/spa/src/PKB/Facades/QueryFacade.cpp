#include "QueryFacade.h"

bool QueryFacade::validateWildcard(Reference leftRef, Reference rightRef,
                                   Solvable *sTable, Solvable *pTable) {
    return sTable->validate(leftRef, rightRef) ||
           pTable->validate(leftRef, rightRef);
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

    Solvable *table;

    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        table = this->storage->getTable<FollowsTable>();
        break;
    }
    case RelationshipReference::FOLLOWS_T: {
        table = this->storage->getTable<FollowsTTable>();
        break;
    }
    case RelationshipReference::PARENT: {
        table = this->storage->getTable<ParentTable>();
        break;
    }
    case RelationshipReference::PARENT_T: {
        table = this->storage->getTable<ParentTTable>();
        break;
    }
    case RelationshipReference::MODIFIES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            table = this->storage->getTable<ModifiesSTable>();
            break;
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            table = this->storage->getTable<ModifiesPTable>();
            break;
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            return validateWildcard(leftRef, rightRef,
                                    this->storage->getTable<ModifiesSTable>(),
                                    this->storage->getTable<ModifiesPTable>());
        }
        break;
    }
    case RelationshipReference::USES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            table = this->storage->getTable<UsesSTable>();
            break;
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            table = this->storage->getTable<UsesPTable>();
            break;
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            return validateWildcard(leftRef, rightRef,
                                    this->storage->getTable<UsesSTable>(),
                                    this->storage->getTable<UsesPTable>());
        }
        break;
    }
    case RelationshipReference::CALLS: {
        table = this->storage->getTable<CallsTable>();
        break;
    }
    case RelationshipReference::CALLS_T: {
        table = this->storage->getTable<CallsTTable>();
        break;
    }
    default: {
        // TODO: throw error instead of return false
        return false;
    }
    }
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

    Solvable *table;

    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        table = this->storage->getTable<FollowsTable>();
        break;
    }
    case RelationshipReference::FOLLOWS_T: {
        table = this->storage->getTable<FollowsTTable>();
        break;
    }
    case RelationshipReference::PARENT: {
        table = this->storage->getTable<ParentTable>();
        break;
    }
    case RelationshipReference::PARENT_T: {
        table = this->storage->getTable<ParentTTable>();
        break;
    }
    case RelationshipReference::MODIFIES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            table = this->storage->getTable<ModifiesSTable>();
            break;
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            table = this->storage->getTable<ModifiesPTable>();
            break;
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            Solvable *modifiesS = this->storage->getTable<ModifiesSTable>();
            Solvable *modifiesP = this->storage->getTable<ModifiesPTable>();
            std::vector<Value> stmtRes =
                modifiesS->solveRight(leftRef, rightSynonym, this->storage->getStorageView());
            std::vector<Value> procRes =
                modifiesP->solveRight(leftRef, rightSynonym, this->storage->getStorageView());
            std::vector<Value> result(stmtRes);
            result.insert(result.end(), procRes.begin(), procRes.end());
            return result;
        }
        break;
    }
    case RelationshipReference::USES: {
        if (leftRef.type == ReferenceType::STMT_REF) {
            table = this->storage->getTable<UsesSTable>();
            break;
        }
        if (leftRef.type == ReferenceType::ENT_REF) {
            table = this->storage->getTable<UsesPTable>();
            break;
        }
        if (leftRef.type == ReferenceType::WILDCARD) {
            Solvable *usesS = this->storage->getTable<UsesSTable>();
            Solvable *usesP = this->storage->getTable<UsesPTable>();
            std::vector<Value> stmtRes =
                usesS->solveRight(leftRef, rightSynonym, this->storage->getStorageView());
            std::vector<Value> procRes =
                usesP->solveRight(leftRef, rightSynonym, this->storage->getStorageView());
            std::vector<Value> result(stmtRes);
            result.insert(result.end(), procRes.begin(), procRes.end());
            return result;
        }
        break;
    }
    case RelationshipReference::CALLS: {
        table = this->storage->getTable<CallsTable>();
        break;
    }
    case RelationshipReference::CALLS_T: {
        table = this->storage->getTable<CallsTTable>();
        break;
    }
    default: {
        // TODO: throw error instead of return false
        return std::vector<Value>();
    }
    }
    return table->solveRight(leftRef, rightSynonym, this->storage->getStorageView());
}

std::vector<Value> QueryFacade::solveLeft(RelationshipReference relType,
                                          Reference rightRef,
                                          EntityName leftSynonym) {
    if (rightRef.isSynonym) {
        // TODO: throw error or remove if we can assume ref passed is always
        // correct
        return std::vector<Value>();
    }
    Solvable *table;

    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        table = this->storage->getTable<FollowsTable>();
        break;
    }
    case RelationshipReference::FOLLOWS_T: {
        table = this->storage->getTable<FollowsTTable>();
        break;
    }
    case RelationshipReference::PARENT: {
        table = this->storage->getTable<ParentTable>();
        break;
    }
    case RelationshipReference::PARENT_T: {
        table = this->storage->getTable<ParentTTable>();
        break;
    }
    case RelationshipReference::MODIFIES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            table = this->storage->getTable<ModifiesSTable>();
            break;
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            table = this->storage->getTable<ModifiesPTable>();
            break;
        }
        // TODO: throw error instead of returning empty list
        return std::vector<Value>();
    }
    case RelationshipReference::USES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            table = this->storage->getTable<UsesSTable>();
            break;
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            table = this->storage->getTable<UsesPTable>();
            break;
        }
        // TODO: throw error instead of returning empty list
        return std::vector<Value>();
    }
    case RelationshipReference::CALLS: {
        table = this->storage->getTable<CallsTable>();
        break;
    }
    case RelationshipReference::CALLS_T: {
        table = this->storage->getTable<CallsTTable>();
        break;
    }
    default: {
        // TODO: throw error instead of return false
        return std::vector<Value>();
    }
    }
    return table->solveLeft(rightRef, leftSynonym, this->storage->getStorageView());
}

std::vector<std::pair<Value, Value>>
QueryFacade::solveBoth(RelationshipReference relType, EntityName leftSynonym,
                       EntityName rightSynonym) {
    Solvable *table;

    switch (relType) {
    case RelationshipReference::FOLLOWS: {
        table = this->storage->getTable<FollowsTable>();
        break;
    }
    case RelationshipReference::FOLLOWS_T: {
        table = this->storage->getTable<FollowsTTable>();
        break;
    }
    case RelationshipReference::PARENT: {
        table = this->storage->getTable<ParentTable>();
        break;
    }
    case RelationshipReference::PARENT_T: {
        table = this->storage->getTable<ParentTTable>();
        break;
    }
    case RelationshipReference::MODIFIES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            table = this->storage->getTable<ModifiesSTable>();
            break;
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            table = this->storage->getTable<ModifiesPTable>();
            break;
        }
        // TODO: throw error instead of returning empty list
        return std::vector<std::pair<Value, Value>>();
    }
    case RelationshipReference::USES: {
        if (stmtRefSet.count(leftSynonym) == 1) {
            table = this->storage->getTable<UsesSTable>();
            break;
        }
        if (leftSynonym == EntityName::PROCEDURE) {
            table = this->storage->getTable<UsesPTable>();
            break;
        }
        // TODO: throw error instead of returning empty list
        return std::vector<std::pair<Value, Value>>();
    }
    case RelationshipReference::CALLS: {
        table = this->storage->getTable<CallsTable>();
        break;
    }
    case RelationshipReference::CALLS_T: {
        table = this->storage->getTable<CallsTTable>();
        break;
    }
    default: {
        // TODO: throw error instead of return false
        return std::vector<std::pair<Value, Value>>();
    }
    }
    return table->solveBoth(leftSynonym, rightSynonym, this->storage->getStorageView());
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
