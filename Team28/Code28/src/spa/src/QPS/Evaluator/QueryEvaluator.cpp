#include "QueryEvaluator.h"

QueryResult QueryEvaluator::evaluate(SolvableQuery *solvableQ) {
    ClauseResult suchThatResult = suchThatEvaluator.evaluate(&solvableQ->suchThatCl);
    ClauseResult patternResult = patternEvaluator.evaluate(&solvableQ->patternCl);
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
        if (queryResult->selectType.entity == EntityName::STMT) {
            std::vector<Statement*> statementList = (std::vector<Statement*>)this->queryFacade->getAllStatements();
            std::vector<std::string> result;
            for (int i = 0; i < statementList.size(); i++) {
                result.push_back(std::to_string(statementList[i]->getLineNumber()));
            }
            return result;
        }
        else if (queryResult->selectType.entity == EntityName::VARIABLE) {
            std::vector<std::string> variableList = this->queryFacade->getAllVariables();
            return variableList;
        }
        else if (queryResult->selectType.entity == EntityName::CONSTANT) {
            std::vector<std::string> constantList = this->queryFacade->getAllConstants();
            return constantList;
        }
        else if (queryResult->selectType.entity == EntityName::PROCEDURE) {
            std::vector<std::string> procedureList = this->queryFacade->getAllProcedures();
            return procedureList;
        }
    }
}