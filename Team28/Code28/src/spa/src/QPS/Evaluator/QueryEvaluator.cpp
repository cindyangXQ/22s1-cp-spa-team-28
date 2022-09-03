#include "QueryEvaluator.h"

QueryResult QueryEvaluator::evaluate(SolvableQuery *solvableQ) {
    ClauseResult suchThatResult = this->suchThatEvaluator.evaluate(solvableQ->suchThatCl);
    ClauseResult patternResult = this->patternEvaluator.evaluate(solvableQ->patternCl);
    std::vector<ClauseResult> clauseResultList{suchThatResult, patternResult};
    return QueryResult(solvableQ->selectType, clauseResultList);
}

std::vector<std::string> QueryEvaluator::interpretQueryResult(QueryResult *queryResult) {
    bool isQueryResultAllTrue = true;
    std::vector<ClauseResult> clauseResultList = queryResult->clauseResultList;
    for (int i = 0; i < clauseResultList.size(); i++) {
        if (clauseResultList[i].isTrue == false) {
            isQueryResultAllTrue = false;
            break;
        }
    }
    if (isQueryResultAllTrue == true) {
        if (queryResult->selectType.entity == DesignEntity::STATEMENT) {
            std::vector<Statement*> statementList = this->queryFacade->getAllStatements();
            std::vector<std::string> result;
            for (int i = 0; i < statementList.size(); i++) {
                result.push_back(std::to_string(statementList[i]->getLineNumber());
            }
            return result;
        }
        else if (queryResult->selectType.entity == DesignEntity::VARIABLE) {
            std::vector<VariableName> variableList = this->queryFacade->getAllVariables();
            return variableList;
        }
        else if (queryResult->selectType.entity == DesignEntity::CONSTANT) {
            std::vector<ConstantName> constantList = this->queryFacade->getAllConstants();
            return constantList;
        }
        else if (queryResult->selectType.entity == DesignEntity::PROCEDURE) {
            std::vector<ProcedureName> procedureList = this->queryFacade->getAllProcedures();
            return procedureList;
        }
    }
}