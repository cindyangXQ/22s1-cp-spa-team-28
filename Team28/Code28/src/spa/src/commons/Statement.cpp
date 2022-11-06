#include "Statement.h"

#include <iostream>

std::map<EntityName, StatementType> Statement::ENTITY_TO_STATEMENT_MAP = {
    {EntityName::READ, StatementType::READ},
    {EntityName::PRINT, StatementType::PRINT},
    {EntityName::CALL, StatementType::CALL},
    {EntityName::WHILE, StatementType::WHILE},
    {EntityName::IF, StatementType::IF},
    {EntityName::ASSIGN, StatementType::ASSIGN},
};

int Statement::getLineNumberFromStatement(Statement *statement) {
    return statement->getLineNumber();
}

Statement::Statement(int lineNo, StatementType type) {
    this->type = type;
    this->lineNo = lineNo;
}

Statement::Statement(int lineNo) { this->lineNo = lineNo; }

StatementType Statement::getStatementType() { return this->type; }

int Statement::getLineNumber() { return this->lineNo; }

StatementType Statement::getStmtTypeFromEntityName(EntityName entityName) {
    return Statement::ENTITY_TO_STATEMENT_MAP[entityName];
}

bool Statement::operator==(const Statement &other) const {
    return this->lineNo == other.lineNo && this->type == other.type;
}

bool Statement::isStatementTypeEqual(Statement *other) {
    return this->type == other->type;
}

bool Statement::isLineNumberEqual(Statement *other) {
    return this->lineNo == other->lineNo;
}
