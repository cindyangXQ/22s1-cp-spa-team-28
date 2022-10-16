#include "QueryEvaluator.h"

QueryResult QueryEvaluator::evaluate(SolvableQuery *solvableQ) {
    std::vector<ClauseResult> clauseResultList;
    for (size_t i = 0; i < solvableQ->suchThatCls.size(); i++) {
        ClauseResult suchThatResult =
            suchThatEvaluator.evaluate(&solvableQ->suchThatCls[i]);
        clauseResultList.push_back(suchThatResult);
    }
    for (size_t i = 0; i < solvableQ->patternCls.size(); i++) {
        ClauseResult patternResult =
            patternEvaluator.evaluate(&solvableQ->patternCls[i]);
        clauseResultList.push_back(patternResult);
    }
    return QueryResult(solvableQ->selectClause, clauseResultList);
}

// TOFIX: checks can be abstracted out so logical flow is easier to follow
// TOFIX: perhaps polymorphism can replace switching on types
std::vector<std::string>
QueryEvaluator::interpretQueryResult(QueryResult *queryResult) {
    std::vector<ClauseResult> clauseResultList = queryResult->clauseResultList;
    SelectType type = queryResult->selectClause.selectType;
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
        Reference selectedRef = queryResult->selectClause.refs[0];
        return extractReferenceFromTable(selectedRef, result);
    } else {
        std::vector<Reference> selectRefs = queryResult->selectClause.refs;
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
        Tuple row = result.rows[i];
        for (int j = 0; j < indices.size(); j++) {
            Value v = row.values[indices[j]];
            tuple += getAttributeValue(selectRefs[j], v.value) + " ";
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
                getAttributeValue(selectedRef, selectValues[k].value));
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
        if (clauseResultList[i].isEmpty) {
            *isAnyTableEmpty = true;
            return;
        }
        if (clauseResultList[i].table.header.size() > 0) {
            *haveTableToJoin = true;
        }
    }
}

std::vector<std::string>
QueryEvaluator::handleNoTables(QueryResult *queryResult) {
    SelectType type = queryResult->selectClause.selectType;
    if (type == SelectType::BOOLEAN) {
        return {"TRUE"};
    } else if (type == SelectType::SINGLE) {
        Reference selectRef = queryResult->selectClause.refs[0];
        std::vector<std::string> synonymValues =
            QueryEvaluator::getAll(selectRef);

        std::vector<std::string> result;
        for (int i = 0; i < synonymValues.size(); i++) {
            result.push_back(getAttributeValue(selectRef, synonymValues[i]));
        }
        return result;
    } else {
        std::vector<Reference> selectedRefs = queryResult->selectClause.refs;
        ClauseTable result = ClauseTable();
        return extractTuplesFromTable(selectedRefs, result);
    }
}

bool QueryEvaluator::isAlternativeAttribute(Reference ref) {
    return (ref.syn.entity == EntityName::PRINT &&
                ref.attr == EntityAttribute::VAR_NAME ||
            ref.syn.entity == EntityName::READ &&
                ref.attr == EntityAttribute::VAR_NAME ||
            ref.syn.entity == EntityName::CALL &&
                ref.attr == EntityAttribute::PROC_NAME);
}

std::string QueryEvaluator::getAttributeValue(Reference ref,
                                              std::string synonymValue) {
    if (isAlternativeAttribute(ref)) {
        return this->queryFacade->getAttribute(std::stoi(synonymValue));
    } else {
        return synonymValue;
    }
}

ClauseTable QueryEvaluator::joinAllClauseTables(
    std::vector<ClauseResult> clauseResultList) {
    ClauseTable result = ClauseTable();
    for (int i = 0; i < clauseResultList.size(); i++) {
        result = ClauseTable::joinTables(result, clauseResultList[i].table);
    }
    return result;
}

// TOFIX: polymorphism
std::vector<std::string> QueryEvaluator::getAll(Reference select) {
    EntityName type = select.syn.entity;
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
