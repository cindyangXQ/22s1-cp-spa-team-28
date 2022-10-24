#include "QueryEvaluator.h"

QueryResult QueryEvaluator::evaluate(SolvableQuery *solvableQ) {
    std::vector<ClauseResult> clauseResultList;
    std::vector<SuchThatClause> suchThatCls = solvableQ->getSuchThatCls();
    std::vector<PatternClause> patternCls = solvableQ->getPatternCls();
    std::vector<WithClause> withCls = solvableQ->getWithCls();
    SelectClause selectClause = solvableQ->getSelectClause();

    for (size_t i = 0; i < suchThatCls.size(); i++) {
        ClauseResult suchThatResult =
            suchThatEvaluator.evaluate(&suchThatCls[i]);
        clauseResultList.push_back(suchThatResult);
    }
    for (size_t i = 0; i < patternCls.size(); i++) {
        ClauseResult patternResult = patternEvaluator.evaluate(&patternCls[i]);
        clauseResultList.push_back(patternResult);
    }
    for (size_t i = 0; i < withCls.size(); i++) {
        ClauseResult withResult = withEvaluator.evaluate(&withCls[i]);
        clauseResultList.push_back(withResult);
    }
    return QueryResult(selectClause, clauseResultList);
}

std::vector<std::string>
QueryEvaluator::interpretQueryResult(QueryResult *queryResult) {
    std::vector<ClauseResult> clauseResultList =
        queryResult->getClauseResultList();
    SelectClause selectClause = queryResult->getSelectClause();
    SelectType type = selectClause.getSelectType();
    bool haveTableToJoin = false;
    bool isAnyTableEmpty = false;

    checkAllClauseResult(clauseResultList, &isAnyTableEmpty, &haveTableToJoin);

    if (isAnyTableEmpty) {
        if (type == SelectType::BOOLEAN) {
            return {"FALSE"};
        } else {
            return {};
        }
    }

    if (!haveTableToJoin) {
        return handleNoTables(queryResult);
    }

    ClauseTable result = joinAllClauseTables(clauseResultList);

    if (result.size() == 0 && type == SelectType::BOOLEAN) {
        return {"FALSE"};
    } else if (result.size() == 0) {
        return std::vector<std::string>{};
    } else if (type == SelectType::BOOLEAN) {
        return {"TRUE"};
    } else if (type == SelectType::SINGLE) {
        Reference selectedRef = selectClause.getRefs()[0];
        return extractReferenceFromTable(selectedRef, result);
    } else {
        std::vector<Reference> selectRefs = selectClause.getRefs();
        return extractTuplesFromTable(selectRefs, result);
    }
}

std::vector<std::string>
QueryEvaluator::extractTuplesFromTable(std::vector<Reference> selectRefs,
                                       ClauseTable result) {
    for (int i = 0; i < selectRefs.size(); i++) {
        ClauseTable table = ClauseTable({selectRefs[i]});
        std::vector<std::string> allValues =
            QueryEvaluator::getAll(selectRefs[i]);
        for (int j = 0; j < allValues.size(); j++) {
            table.insert(Tuple({Value(ValueType::WILDCARD, allValues[j])}));
        }

        result = ClauseTable::joinTables(result, table);
    }
    std::vector<int> indices = result.getIndices(selectRefs);
    std::vector<std::string> output;
    for (int i = 0; i < result.size(); i++) {
        std::string tuple = "";
        Tuple row = result.getRows()[i];
        for (int j = 0; j < indices.size(); j++) {
            Value v = row.getValues()[indices[j]];
            tuple += getAttributeValue(selectRefs[j], v.getValue()) + " ";
        }
        output.push_back(Utils::trimSpaces(tuple));
    }
    return output;
}

std::vector<std::string>
QueryEvaluator::extractReferenceFromTable(Reference selectedRef,
                                          ClauseTable result) {

    std::vector<Value> selectValues = result.getValues(selectedRef);
    if (selectValues.size() == 0) {
        std::vector<std::string> synonymValues =
            QueryEvaluator::getAll(selectedRef);

        std::vector<std::string> result;
        for (int i = 0; i < synonymValues.size(); i++) {
            result.push_back(getAttributeValue(selectedRef, synonymValues[i]));
        }
        return result;
    } else {
        std::unordered_set<std::string> removeDuplicates;
        std::vector<std::string> output;
        for (int k = 0; k < selectValues.size(); k++) {
            removeDuplicates.insert(
                getAttributeValue(selectedRef, selectValues[k].getValue()));
        }
        output.insert(output.end(), removeDuplicates.begin(),
                      removeDuplicates.end());
        return output;
    }
}

void QueryEvaluator::checkAllClauseResult(
    std::vector<ClauseResult> clauseResultList, bool *isAnyTableEmpty,
    bool *haveTableToJoin) {
    for (int i = 0; i < clauseResultList.size(); i++) {
        if (clauseResultList[i].getIsEmpty()) {
            *isAnyTableEmpty = true;
            return;
        }
        ClauseTable table = clauseResultList[i].getTable();
        std::vector<Reference> header = table.getHeader();
        if (header.size() > 0) {
            *haveTableToJoin = true;
        }
    }
}

std::vector<std::string>
QueryEvaluator::handleNoTables(QueryResult *queryResult) {
    SelectClause selectClause = queryResult->getSelectClause();
    SelectType type = selectClause.getSelectType();
    if (type == SelectType::BOOLEAN) {
        return {"TRUE"};
    } else if (type == SelectType::SINGLE) {
        Reference selectRef = selectClause.getRefs()[0];
        std::vector<std::string> synonymValues =
            QueryEvaluator::getAll(selectRef);

        std::vector<std::string> result;
        for (int i = 0; i < synonymValues.size(); i++) {
            result.push_back(getAttributeValue(selectRef, synonymValues[i]));
        }
        return result;
    } else {
        std::vector<Reference> selectedRefs = selectClause.getRefs();
        ClauseTable result = ClauseTable();
        return extractTuplesFromTable(selectedRefs, result);
    }
}

bool QueryEvaluator::isAlternativeAttribute(Reference ref) {
    return (ref.getEntityName() == EntityName::PRINT &&
                ref.getAttr() == EntityAttribute::VAR_NAME ||
            ref.getEntityName() == EntityName::READ &&
                ref.getAttr() == EntityAttribute::VAR_NAME ||
            ref.getEntityName() == EntityName::CALL &&
                ref.getAttr() == EntityAttribute::PROC_NAME);
}

std::string QueryEvaluator::getAttributeValue(Reference ref,
                                              std::string synonymValue) {
    if (isAlternativeAttribute(ref)) {
        return this->queryFacade->getSecondaryAttribute(
            std::stoi(synonymValue));
    } else {
        return synonymValue;
    }
}

ClauseTable QueryEvaluator::joinAllClauseTables(
    std::vector<ClauseResult> clauseResultList) {
    ClauseTable result = ClauseTable();
    for (int i = 0; i < clauseResultList.size(); i++) {
        result =
            ClauseTable::joinTables(result, clauseResultList[i].getTable());
    }
    return result;
}

std::vector<std::string> QueryEvaluator::getAll(Reference select) {
    EntityName type = select.getEntityName();
    if (type == EntityName::STMT) {
        std::vector<Statement *> statementList =
            (std::vector<Statement *>)this->queryFacade->getAllStatementsByType(
                StatementType::STMT);
        std::vector<std::string> result;
        for (int i = 0; i < statementList.size(); i++) {
            result.push_back(std::to_string(statementList[i]->getLineNumber()));
        }
        return result;
    } else if (type == EntityName::VARIABLE) {
        std::vector<std::string> variableList =
            this->queryFacade->getAllEntities(Designation::VAR);
        return variableList;
    } else if (type == EntityName::CONSTANT) {
        std::vector<std::string> constantList =
            this->queryFacade->getAllEntities(Designation::CONST);
        return constantList;
    } else if (type == EntityName::PROCEDURE) {
        std::vector<std::string> procedureList =
            this->queryFacade->getAllEntities(Designation::PROC);
        return procedureList;
    } else {
        StatementType stmtType = Statement::getStmtTypeFromEntityName(type);
        std::vector<Statement *> statementList =
            (std::vector<Statement *>)this->queryFacade->getAllStatementsByType(
                stmtType);
        std::vector<std::string> result;
        for (int i = 0; i < statementList.size(); i++) {
            result.push_back(std::to_string(statementList[i]->getLineNumber()));
        }
        return result;
    }
}
