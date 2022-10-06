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
        EntityName type = queryResult->selectClause.syns[0].entity;
        if (type == EntityName::STMT) {
            std::vector<Statement *> statementList =
                (std::vector<Statement *>)this->queryFacade->getAllStatements();
            std::vector<std::string> result;
            for (int i = 0; i < statementList.size(); i++) {
                result.push_back(
                    std::to_string(statementList[i]->getLineNumber()));
            }
            return result;
        } else if (type == EntityName::VARIABLE) {
            std::vector<std::string> variableList =
                this->queryFacade->getAllVariables();
            return variableList;
        } else if (type == EntityName::CONSTANT) {
            std::vector<std::string> constantList =
                this->queryFacade->getAllConstants();
            return constantList;
        } else if (type == EntityName::PROCEDURE) {
            std::vector<std::string> procedureList =
                this->queryFacade->getAllProcedures();
            return procedureList;
        } else {
            StatementType stmtType = Statement::getStmtTypeFromEntityName(type);
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
        Synonym selectedSynonym = queryResult->selectClause.syns[0];
        EntityName type = selectedSynonym.entity;
        std::vector<Value> selectValues =
            result.getValues(selectedSynonym);
        if (selectValues.size() == 0) {
            if (type == EntityName::STMT) {
                std::vector<Statement *> statementList =
                    (std::vector<Statement *>)this->queryFacade
                        ->getAllStatements();
                std::vector<std::string> result;
                for (int i = 0; i < statementList.size(); i++) {
                    result.push_back(
                        std::to_string(statementList[i]->getLineNumber()));
                }
                return result;
            } else if (type == EntityName::VARIABLE) {
                std::vector<std::string> variableList =
                    this->queryFacade->getAllVariables();
                return variableList;
            } else if (type == EntityName::CONSTANT) {
                std::vector<std::string> constantList =
                    this->queryFacade->getAllConstants();
                return constantList;
            } else if (type == EntityName::PROCEDURE) {
                std::vector<std::string> procedureList =
                    this->queryFacade->getAllProcedures();
                return procedureList;
            } else {
                StatementType stmtType = Statement::getStmtTypeFromEntityName(type);
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
            std::unordered_set<std::string> remove_duplicates;
            std::vector<std::string> output;
            for (int k = 0; k < selectValues.size(); k++) {
                remove_duplicates.insert(selectValues[k].value);
            }
            output.insert(output.end(), remove_duplicates.begin(),
                          remove_duplicates.end());
            return output;
        }
    }
}
