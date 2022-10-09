#include "StatementsTable.h"

#include <algorithm>

StatementsTable::StatementsTable() = default;

void StatementsTable::store(TableValue *statement) {
    Statement *stmt = static_cast<Statement *>(statement);
    StatementType type = stmt->getStatementType();
    int index = stmt->getLineNumber();

    if (type == StatementType::NONE) {
        // TODO error handling
    }

    this->statements.push_back(stmt);
    this->statementTypeIndexes[type].push_back(index);
}

Statement *StatementsTable::retrieve(const int &lineNum) {
    // currently assumes that lineNum is unique
    for (Statement *statement : this->statements) {
        if (statement->getLineNumber() == lineNum) {
            return statement;
        }
    }

    return nullptr;
}

StatementType StatementsTable::getStatementType(const int &lineNum) {
    // currently assumes that lineNum is unique
    for (Statement *statement : this->statements) {
        if (statement->getLineNumber() == lineNum) {
            return statement->getStatementType();
        }
    }

    return StatementType::NONE;
}

std::vector<Statement *> StatementsTable::getAll() { return this->statements; }

std::vector<int> StatementsTable::getAllLineNumbers() {
    std::vector<Statement *> statements = this->statements;
    std::vector<int> lineNumbers;
    std::transform(statements.begin(), statements.end(),
                   std::back_inserter(lineNumbers),
                   Statement::getLineNumberFromStatement);
    return lineNumbers;
}

std::vector<int> StatementsTable::getStatementsByType(StatementType type) {
    return this->statementTypeIndexes[type];
}
