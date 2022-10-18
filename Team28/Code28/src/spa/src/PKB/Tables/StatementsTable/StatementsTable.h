#pragma once

#include <map>
#include <string>
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
     * Returns string form of all statements.
     */
    std::vector<std::string> getAllAsString();

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

    bool isIfStatement(const int &index);

    /*
     * Returns a boolean of whether the given statement is one of Calls, Print
     * or Read, which have a secondary attribute (varName/procName).
     */
    bool hasSecondaryAttribute(const int &index);

    /*
     * Get table size.
     * TODO check if this can be removed. Only used for Next* currently.
     */
    int getTableSize();

    std::vector<Value> getMatchingValue(std::string value, EntityName entity);

    std::map<Value, std::vector<Value>> getAllValues(EntityName entity);

private:
    int tableSize = 0;
    const std::string STMT_TYPE_NONE_INVALID_USE =
        "StatementType cannot be NONE";

    std::map<StatementType, std::vector<int>> statementTypeIndexes = {
        {StatementType::READ, {}},   {StatementType::PRINT, {}},
        {StatementType::ASSIGN, {}}, {StatementType::CALL, {}},
        {StatementType::WHILE, {}},  {StatementType::IF, {}},
    };

    std::vector<Statement *> statements;
};
