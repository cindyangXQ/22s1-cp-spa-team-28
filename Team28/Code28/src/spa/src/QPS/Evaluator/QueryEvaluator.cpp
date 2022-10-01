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
    return QueryResult(solvableQ->selectType, clauseResultList);
}

std::vector<std::string>
QueryEvaluator::interpretQueryResult(QueryResult *queryResult) {
    bool allClauseResultEmpty = true;
    std::vector<ClauseResult> clauseResultList = queryResult->clauseResultList;
    for (int i = 0; i < clauseResultList.size(); i++) {
        if (clauseResultList[i].isEmpty) {
            return std::vector<std::string>{};
        }
        if (clauseResultList[i].table.size() > 0) {
            allClauseResultEmpty = false;
        }
    }

    if (allClauseResultEmpty || clauseResultList.size() == 0) {
        if (queryResult->selectType.entity == EntityName::STMT) {
            std::vector<Statement *> statementList =
                (std::vector<Statement *>)this->queryFacade->getAllStatements();
            std::vector<std::string> result;
            for (int i = 0; i < statementList.size(); i++) {
                result.push_back(
                    std::to_string(statementList[i]->getLineNumber()));
            }
            return result;
        } else if (queryResult->selectType.entity == EntityName::VARIABLE) {
            std::vector<std::string> variableList =
                this->queryFacade->getAllVariables();
            return variableList;
        } else if (queryResult->selectType.entity == EntityName::CONSTANT) {
            std::vector<std::string> constantList =
                this->queryFacade->getAllConstants();
            return constantList;
        } else if (queryResult->selectType.entity == EntityName::PROCEDURE) {
            std::vector<std::string> procedureList =
                this->queryFacade->getAllProcedures();
            return procedureList;
        } else {
            StatementType stmtType = Statement::getStmtTypeFromEntityName(
                queryResult->selectType.entity);
            std::vector<Statement *> statementList =
                (std::vector<Statement *>)this->queryFacade
                    ->getAllStatementsByType(stmtType);
            std::vector<std::string> result;
            for (int i = 0; i < statementList.size(); i++) {
                result.push_back(
                    std::to_string(statementList[i]->getLineNumber()));
            }
            return result;
        }
    }
    ClauseTable result = clauseResultList[0].table;
    for (int i = 1; i < clauseResultList.size(); i++) {
        result = ClauseTable::joinTables(result, clauseResultList[i].table);
    }
    if (result.size() == 0) {
        return std::vector<std::string>{};
    } else {
        std::vector<Value> selectValues =
            result.getValues(queryResult->selectType);
        if (selectValues.size() == 0) {
            if (queryResult->selectType.entity == EntityName::STMT) {
                std::vector<Statement *> statementList =
                    (std::vector<Statement *>)this->queryFacade
                        ->getAllStatements();
                std::vector<std::string> result;
                for (int i = 0; i < statementList.size(); i++) {
                    result.push_back(
                        std::to_string(statementList[i]->getLineNumber()));
                }
                return result;
            } else if (queryResult->selectType.entity == EntityName::VARIABLE) {
                std::vector<std::string> variableList =
                    this->queryFacade->getAllVariables();
                return variableList;
            } else if (queryResult->selectType.entity == EntityName::CONSTANT) {
                std::vector<std::string> constantList =
                    this->queryFacade->getAllConstants();
                return constantList;
            } else if (queryResult->selectType.entity ==
                       EntityName::PROCEDURE) {
                std::vector<std::string> procedureList =
                    this->queryFacade->getAllProcedures();
                return procedureList;
            } else {
                StatementType stmtType = Statement::getStmtTypeFromEntityName(
                    queryResult->selectType.entity);
                std::vector<Statement *> statementList =
                    (std::vector<Statement *>)this->queryFacade
                        ->getAllStatementsByType(stmtType);
                std::vector<std::string> result;
                for (int i = 0; i < statementList.size(); i++) {
                    result.push_back(
                        std::to_string(statementList[i]->getLineNumber()));
                }
                return result;
            }
        } else {
            std::vector<std::string> output;
            for (int k = 0; k < selectValues.size(); k++) {
                output.push_back(selectValues[k].value);
            }
            return output;
        }
    }
}
