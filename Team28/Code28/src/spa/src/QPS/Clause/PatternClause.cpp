#include "PatternClause.h"

Reference PatternClause::getStmtRef() { return this->stmtRef; }
Reference PatternClause::getEntRef() { return this->entRef; }
Expression PatternClause::getExpression() { return this->expression; }
bool PatternClause::getIsExact() { return this->isExact; }

void PatternClause::parse(std::smatch matches, std::vector<Synonym> syns) {
    this->stmtRef = Reference::getReference(matches[2], syns);
    if (stmtRef.getEntityName() == EntityName::IF) {
        if (!std::regex_search(matches[6].str(), IF_PATTERN)) {
            throw SyntaxError("Too many wildcards at the end of If pattern");
        }
    } else if (stmtRef.getEntityName() == EntityName::ASSIGN ||
               stmtRef.getEntityName() == EntityName::WHILE) {
        if (!std::regex_search(matches[6].str(), ASSIGN_WHILE_PATTERN)) {
            throw SyntaxError(
                "Too many wildcards at the end of Assign or while pattern");
        }
    }
    this->entRef = Reference::getReference(matches[3], syns);
    Expression expr = Utils::trimSpaces(matches[5]);
    this->isExact = expr.find('_') == std::string::npos;
    this->populateSynsUsed();

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
    this->patternType =
        ENTITY_DESIGNATION_MAP.at(this->stmtRef.getEntityName());
    return true;
}

ClauseResult PatternClause::evaluate(QueryFacade *queryFacade) {
    if (stmtRef.isASynonym() && entRef.isASynonym()) {
        return handleBothSynonym(queryFacade);
    } else if (!stmtRef.isASynonym() && entRef.isASynonym()) {
        return handleRightSynonym(queryFacade);
    } else if (stmtRef.isASynonym() && !entRef.isASynonym()) {
        return handleLeftSynonym(queryFacade);
    } else if (!stmtRef.isASynonym() && !entRef.isASynonym()) {
        return handleNoSynonym(queryFacade);
    }
}

ClauseResult PatternClause::handleNoSynonym(QueryFacade *queryFacade) {
    bool isTrue;
    bool isEmpty;
    if (this->patternType == Designation::ASSIGN) {
        AssignExpression expr = AssignExpression(expression, isExact);
        isTrue = queryFacade->validate(std::stoi(stmtRef.getValueString()),
                                       entRef.getValueString(), expr);
    } else {
        isTrue = queryFacade->validate(patternType,
                                       std::stoi(stmtRef.getValueString()),
                                       entRef.getValueString());
    }
    isEmpty = !isTrue;
    return ClauseResult(isEmpty);
}

ClauseResult PatternClause::handleLeftSynonym(QueryFacade *queryFacade) {
    if (this->patternType == Designation::ASSIGN) {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef});
        AssignExpression expr = AssignExpression(expression, isExact);
        std::vector<Value> result =
            queryFacade->getAssign(entRef.getValueString(), expr);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef});
        std::vector<Value> result =
            queryFacade->getCond(patternType, entRef.getValueString());
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternClause::handleRightSynonym(QueryFacade *queryFacade) {
    if (this->patternType == Designation::ASSIGN) {
        ClauseResult clauseResult = ClauseResult(std::vector{entRef});
        AssignExpression expr = AssignExpression(expression, isExact);
        std::vector<Value> result =
            queryFacade->getVar(std::stoi(stmtRef.getValueString()), expr);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    } else {
        ClauseResult clauseResult = ClauseResult(std::vector{entRef});
        std::vector<Value> result = queryFacade->getVar(
            patternType, std::stoi(stmtRef.getValueString()));
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(Tuple(std::vector{result[i]}));
        }
        return clauseResult;
    }
}

ClauseResult PatternClause::handleBothSynonym(QueryFacade *queryFacade) {
    if (this->patternType == Designation::ASSIGN) {
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
        ClauseResult clauseResult = ClauseResult(std::vector{stmtRef, entRef});
        std::vector<std::pair<Value, Value>> result =
            queryFacade->getCondAndVar(patternType);
        for (int i = 0; i < result.size(); i++) {
            clauseResult.insert(
                Tuple(std::vector{result[i].first, result[i].second}));
        }
        return clauseResult;
    }
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

void PatternClause::populateOptimizeScore(QueryFacade *queryFacade) {
    double multiplier = 1.0;
    if (synsUsed.size() == 0) {
        multiplier *= 0.01;
    }
    double baseScore = 0.0;
    baseScore += queryFacade->getTableSize(patternType);
    if (this->stmtRef.isASynonym()) {
        baseScore += queryFacade->getTableSize(this->stmtRef.getDesignation());
    }
    if (this->entRef.isASynonym()) {
        baseScore += queryFacade->getTableSize(this->entRef.getDesignation());
    }
    if (multiplier * baseScore < 0) {
        this->score = INT_MAX;
    } else {
        this->score = multiplier * baseScore;
    }
}

double PatternClause::getOptimizeScore() { return this->score; }

bool PatternClause::replace(Reference synRef, Reference valRef) {
    bool replaced = false;
    if (entRef.isASynonym() &&
        entRef.getSynonymName() == synRef.getSynonymName()) {
        entRef = valRef;
        replaced = true;
    }
    if (stmtRef.isASynonym() &&
        stmtRef.getSynonymName() == synRef.getSynonymName()) {
        stmtRef = valRef;
        replaced = true;
    }
    return replaced;
}