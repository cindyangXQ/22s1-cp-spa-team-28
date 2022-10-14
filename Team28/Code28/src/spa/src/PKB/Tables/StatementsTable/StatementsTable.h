#pragma once

#include <map>
#include <vector>

#include "../../../commons/Statement.h"
#include "../Table.h"

/*
 * Class encapsulating a Table used to store SIMPLE statements.
 */
class StatementsTable : public Table {
public:
    StatementsTable();

    /*
     * Stores a statement into the StatementsTable.
     */
    void store(TableValue *statement);

    /*
     * Retrieves a statement from StatementsTable by index.
     */
    Statement *retrieve(const int &index);

    /*
     * Retrieves type of a statement from StatementsTable by index.
     */
    StatementType getStatementType(const int &index);

    /*
     * Gets all statements in StatementsTable.
     */
    std::vector<Statement *> getAll();

    /*
     * Gets all line numbers in StatementsTable.
     */
    std::vector<int> getAllLineNumbers();

    /*
     * Gets all statements of the specified type from StatementsTable.
     */
    std::vector<int> getStatementsByType(StatementType type);

    /*
     * Returns a boolean of whether the given statement is one of Calls, Print
     * or Read.
     */
    bool isAttributableStatement(const int &index);

    std::vector<Value> getValue(int value, EntityName entity);

    std::vector<Value> getValue(std::string value, EntityName entity);

private:
    std::map<StatementType, std::vector<int>> statementTypeIndexes = {
        {StatementType::READ, {}},   {StatementType::PRINT, {}},
        {StatementType::ASSIGN, {}}, {StatementType::CALL, {}},
        {StatementType::WHILE, {}},  {StatementType::IF, {}},
    };

    std::vector<Statement *> statements;
};
