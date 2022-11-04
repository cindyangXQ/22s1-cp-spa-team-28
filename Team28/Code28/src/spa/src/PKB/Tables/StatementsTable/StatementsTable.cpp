#include "StatementsTable.h"

#include "../../../commons/StringUtil.h"
#include <algorithm>

StatementsTable::StatementsTable() = default;

void StatementsTable::store(TableValue *statement) {
    Statement *stmt = static_cast<Statement *>(statement);
    StatementType type = stmt->getStatementType();
    int index = stmt->getLineNumber();

    if (type == StatementType::NONE) {
        throw std::invalid_argument(STMT_TYPE_NONE_INVALID_USE);
    }

    this->statements.push_back(stmt);
    this->statementTypeIndexes[type].push_back(index);
    this->tableSize++;
}

int StatementsTable::getTableSize() { return this->tableSize; }

std::vector<std::string> StatementsTable::getAllAsString() {
    std::vector<std::string> result = {};
    for (Statement *stmt : this->statements) {
        result.push_back(std::to_string(stmt->getLineNumber()));
    }
    return result;
};

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
    if (type == StatementType::STMT) {
        return getAllLineNumbers();
    }
    return this->statementTypeIndexes[type];
}

bool StatementsTable::isIfStatement(const int &lineNum) {
    std::vector<int> ifs = this->getStatementsByType(StatementType::IF);
    return std::find(ifs.begin(), ifs.end(), lineNum) != ifs.end();
}

bool StatementsTable::hasSecondaryAttribute(const int &lineNum) {
    StatementType stmtType = this->getStatementType(lineNum);
    if (stmtType == StatementType::CALL || stmtType == StatementType::READ ||
        stmtType == StatementType::PRINT) {
        return true;
    }
    return false;
}

std::vector<Value> StatementsTable::getMatchingValue(std::string value,
                                                     EntityName entity) {
    int v = std::stoi(value);
    std::vector<int> values;
    std::vector<Value> result = {};
    if (entity == EntityName::STMT) {
        values = this->getAllLineNumbers();
    } else {
        StatementType stmtType = Statement::entityToStatementMap[entity];
        values = this->getStatementsByType(stmtType);
    }
    for (int i : values) {
        if (i == v) {
            result.push_back(Value(ValueType::STMT_NUM, toString(i)));
        }
    }
    return result;
}

std::map<Value, std::vector<Value>>
StatementsTable::getAllValues(EntityName entity) {
    std::vector<int> values;
    std::map<Value, std::vector<Value>> result = {};
    if (entity == EntityName::STMT) {
        values = this->getAllLineNumbers();
    } else {
        StatementType stmtType = Statement::entityToStatementMap[entity];
        values = this->getStatementsByType(stmtType);
    }
    for (int i : values) {
        Value v = Value(ValueType::STMT_NUM, toString(i));
        result[v] = {v};
    }
    return result;
}

std::unordered_set<int>
StatementsTable::getStatementsSetByType(StatementType type) {
    std::vector<int> statements = this->getStatementsByType(type);
    std::unordered_set<int> statementsSet(statements.begin(), statements.end());
    return statementsSet;
};
