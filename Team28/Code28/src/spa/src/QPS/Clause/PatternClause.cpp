#include "PatternClause.h"

Reference PatternClause::getStmtRef() { return this->stmtRef; }
Reference PatternClause::getEntRef() { return this->entRef; }
Expression PatternClause::getExpression() { return this->expression; }
bool PatternClause::getIsExact() { return this->isExact; }

void PatternClause::parse(std::smatch matches, std::vector<Synonym> syns) {
    this->stmtRef = Reference::getReference(matches[2], syns);
    this->entRef = Reference::getReference(matches[3], syns);
    Expression expr = Utils::trimSpaces(matches[5]);
    this->isExact = expr.find('_') == std::string::npos;

    if (expr.find('_') != std::string::npos && expr != "_") {
        // Remove _ at the start and end
        expr = expr.substr(1, expr.size() - 2);
        expr = Utils::trimSpaces(expr);
    }
    if (expr != "_") {
        if (stmtRef.getEntityName() == EntityName::WHILE ||
            stmtRef.getEntityName() == EntityName::IF) {
            throw SyntaxError("Invalid pattern second argument");
        }
        try {
            // Remove " at the start and end
            expr = expr.substr(1, expr.size() - 2);
            expr = Utils::trimSpaces(expr);
            expr = SP::convertExpression(expr);
        } catch (...) {
            throw SyntaxError("Invalid expression syntax");
        }
    }
    this->expression = expr;
}

bool PatternClause::validate() {
    if (!PATTERN_ENTITY_MAP.count(stmtRef.getEntityName())) {
        return false;
    } else if ((entRef.isASynonym() &&
                entRef.getEntityName() != EntityName::VARIABLE) ||
               entRef.getRefType() == ReferenceType::STMT_REF ||
               entRef.getRefType() == ReferenceType::ATTR_REF) {
        return false;
    }
    return true;
}

ClauseResult PatternClause::evaluate(QueryFacade *queryFacade) {
    if (stmtRef.getEntityName() == EntityName::ASSIGN) {
        return handleAssign(queryFacade);
    } else if (stmtRef.getEntityName() == EntityName::WHILE) {
        return handleWhile(queryFacade);
    } else if (stmtRef.getEntityName() == EntityName::IF) {
        return handleIf(queryFacade);
    } else {
        return ClauseResult(true);
    }
}

ClauseResult PatternClause::handleAssign(QueryFacade *queryFacade) {
    if (entRef.isASynonym()) {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef, entRef});
        AssignExpression expr = AssignExpression(expression, isExact);
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getAssignAndVar(expr);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef});
        AssignExpression expr = AssignExpression(expression, isExact);
        std::vector<Value> result =
            queryFacade->getAssign(entRef.getValueString(), expr);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternClause::handleWhile(QueryFacade *queryFacade) {
    if (entRef.isASynonym()) {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef, entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::WHILE_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef});
        std::vector<Value> result =
            queryFacade->getCond(Designation::WHILE_C, entRef.getValueString());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternClause::handleIf(QueryFacade *queryFacade) {
    if (entRef.isASynonym()) {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef, entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::IF_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef});
        std::vector<Value> result =
            queryFacade->getCond(Designation::IF_C, entRef.getValueString());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

void PatternClause::replaceFirstReference(Reference *newRef) {
    this->stmtRef = *newRef;
}

void PatternClause::replaceSecondReference(Reference *newRef) {
    this->entRef = *newRef;
}

std::unordered_set<std::string> PatternClause::getSynonymsUsed() {
    return this->synsUsed;
}

void PatternClause::populateSynsUsed() {
    synsUsed.insert(stmtRef.getSynonymName());
    if (entRef.isASynonym()) {
        synsUsed.insert(entRef.getSynonymName());
    }
}

double PatternClause::getOptimizeScore() {
    double baseScore = 1.0;
    double synScore = 1.0;
    if (this->synsUsed.size() == 0) {
        synScore = 0.01;
    } else if (this->synsUsed.size() == 1) {
        synScore = 0.5;
    }
    return baseScore * synScore;
}