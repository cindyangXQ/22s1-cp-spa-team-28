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
    std::vector<ClauseResult> clauseResultList = queryResult->clauseResultList;
    SelectType type = queryResult->selectClause.selectType;
    bool haveTableToJoin = false;

    for (int i = 0; i < clauseResultList.size(); i++) {
        if (clauseResultList[i].isEmpty && type == SelectType::BOOLEAN) {
            return std::vector<std::string>{"FALSE"};
        } else if (clauseResultList[i].isEmpty) {
            return std::vector<std::string>{};
        }
        if (clauseResultList[i].table.header.size() > 0) {
            haveTableToJoin = true;
        }
    }

    if (!haveTableToJoin) {
        if (type == SelectType::BOOLEAN) {
            return std::vector<std::string>{"TRUE"};
        } else if (type == SelectType::SINGLE) {
            return QueryEvaluator::getAll(queryResult->selectClause.syns[0]);
        } else {
            std::vector<Synonym> selectedSynonyms =
                queryResult->selectClause.syns;
            std::vector<std::vector<std::string>> allResults;
            for (int i = 0; i < selectedSynonyms.size(); i++) {
                allResults.push_back(
                    QueryEvaluator::getAll(selectedSynonyms[i]));
                if (allResults[i].size() == 0) {
                    return {};
                }
            }
            std::vector<std::string> output = allResults[0];
            for (int i = 1; i < allResults.size(); i++) {
                std::vector<std::string> tmp = {};
                for (int j = 0; j < output.size(); j++) {
                    for (int k = 0; k < allResults[i].size(); k++) {
                        tmp.push_back(output[j] + " " + allResults[i][k]);
                    }
                }
                output = tmp;
            }
            return output;
        }
    }

    ClauseTable result = ClauseTable();
    for (int i = 0; i < clauseResultList.size(); i++) {
        result = ClauseTable::joinTables(result, clauseResultList[i].table);
    }

    if (result.size() == 0) {
        if (type == SelectType::BOOLEAN) {
            return {"FALSE"};
        } else {
            return std::vector<std::string>{};
        }
    } else if (type == SelectType::BOOLEAN) {
        return {"TRUE"};
    } else if (type == SelectType::SINGLE) {
        Synonym selectedSynonym = queryResult->selectClause.syns[0];
        std::vector<Value> selectValues = result.getValues(selectedSynonym);
        if (selectValues.size() == 0) {
            return QueryEvaluator::getAll(queryResult->selectClause.syns[0]);
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
    } else {
        std::vector<Synonym> selectSynonyms = queryResult->selectClause.syns;
        for (int i = 0; i < selectSynonyms.size(); i++) {
            ClauseTable table = ClauseTable({selectSynonyms[i]});
            std::vector<std::string> all_values =
                QueryEvaluator::getAll(selectSynonyms[i]);
            for (int j = 0; j < all_values.size(); j++) {
                table.insert(
                    Tuple({Value(ValueType::WILDCARD, all_values[j])}));
            }
            
            result = ClauseTable::joinTables(result, table);
        }
        std::vector<int> indices = result.getIndices(selectSynonyms);
        std::vector<std::string> output;
        for (int i = 0; i < result.size(); i++) {
            std::string tuple = "";
            for (int j = 0; j < indices.size(); j++) {
                tuple += result.rows[i].values[indices[j]].value + " ";
            }
            output.push_back(Utils::removeTrailingSpaces(tuple));            
        }
        return output;
    }
}

std::vector<std::string> QueryEvaluator::getAll(Synonym select) {
    EntityName type = select.entity;
    if (type == EntityName::STMT) {
        std::vector<Statement *> statementList =
            (std::vector<Statement *>)this->queryFacade->getAllStatements();
        std::vector<std::string> result;
        for (int i = 0; i < statementList.size(); i++) {
            result.push_back(std::to_string(statementList[i]->getLineNumber()));
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
            (std::vector<Statement *>)this->queryFacade->getAllStatementsByType(
                stmtType);
        std::vector<std::string> result;
        for (int i = 0; i < statementList.size(); i++) {
            result.push_back(std::to_string(statementList[i]->getLineNumber()));
        }
        return result;
    }
}
