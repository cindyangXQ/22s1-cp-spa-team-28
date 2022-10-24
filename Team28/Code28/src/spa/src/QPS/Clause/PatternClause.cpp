#include "PatternClause.h"

Synonym PatternClause::getSyn() { return this->syn; }
Reference PatternClause::getEntRef() { return this->entRef; }
Expression PatternClause::getExpression() { return this->expression; }
bool PatternClause::getIsExact() { return this->isExact; }

void PatternClause::parse(std::smatch matches, std::vector<Synonym> syns) {
    this->syn = Synonym::getSynonym(matches[2], syns);
    this->entRef = Reference::getReference(matches[3], syns);
    Expression expr = Utils::trimSpaces(matches[5]);
    this->isExact = expr.find('_') == std::string::npos;

    if (expr.find('_') != std::string::npos && expr != "_") {
        // Remove _ at the start and end
        expr = expr.substr(1, expr.size() - 2);
        expr = Utils::trimSpaces(expr);
    }
    if (expr != "_") {
        if (syn.getEntityName() == EntityName::WHILE ||
            syn.getEntityName() == EntityName::IF) {
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
    if (!PATTERN_ENTITY_MAP.count(syn.getEntityName())) {
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
    if (syn.getEntityName() == EntityName::ASSIGN) {
        return handleAssign(queryFacade);
    } else if (syn.getEntityName() == EntityName::WHILE) {
        return handleWhile(queryFacade);
    } else if (syn.getEntityName() == EntityName::IF) {
        return handleIf(queryFacade);
    } else {
        return ClauseResult(true);
    }
}

ClauseResult PatternClause::handleAssign(QueryFacade *queryFacade) {
    if (entRef.isASynonym()) {
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(syn), entRef});
        AssignExpression expr = AssignExpression(expression, isExact);
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getAssignAndVar(expr);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{Reference(syn)});
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
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(syn), entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::WHILE_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{Reference(syn)});
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
        ClauseResult clauseResult =
            ClauseResult(std::vector{Reference(syn), entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(Designation::IF_C);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{Reference(syn)});
        std::vector<Value> result =
            queryFacade->getCond(Designation::IF_C, entRef.getValueString());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}
