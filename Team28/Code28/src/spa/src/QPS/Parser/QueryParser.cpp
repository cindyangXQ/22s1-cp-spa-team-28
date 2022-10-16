#include "QueryParser.h"
#include "../../SP/SP.h"

SolvableQuery QueryParser::parse(std::string query) {
    if (query.back() == ';') {
        throw SyntaxError("Semicolon at the end not allowed");
    }
    std::vector<std::string> clauses = Utils::splitString(query, ';');
    Declaration decl;
    SelectClause selectClause;
    std::vector<SuchThatClause> suchThatCls;
    std::vector<PatternClause> patternCls;
    std::vector<WithClause> withCls;

    if (clauses.size() >= 2) {
        decl = QueryParser::parseDeclaration(clauses);
    }

    // Extract main clause
    std::string mainClause =
        Utils::removeTrailingSpaces(clauses[clauses.size() - 1]);
    selectClause = QueryParser::parseSelectClause(&mainClause, decl.syns);
    while (!Utils::removeTrailingSpaces(mainClause).empty()) {
        if (QueryParser::isSuchThatClause(&mainClause)) {
            QueryParser::parseSuchThatClause(&mainClause, decl.syns,
                                             &suchThatCls);
        } else if (QueryParser::isPatternClause(&mainClause)) {
            QueryParser::parsePatternClause(&mainClause, decl.syns,
                                            &patternCls);
        } else if (QueryParser::isWithClause(&mainClause)) {
            QueryParser::parseWithClause(&mainClause, decl.syns, &withCls);
        } else {
            throw SyntaxError("Unrecognized clause syntax");
        }
    }
    return SolvableQuery(decl, selectClause, suchThatCls, patternCls, withCls);
}

Declaration QueryParser::parseDeclaration(std::vector<std::string> clauses) {
    std::vector<Synonym> all_syns;
    std::vector<Synonym> curr_syns;
    std::vector<std::string> tokens;
    std::vector<char> special_chars{','};

    for (int i = 0; i < clauses.size() - 1; i++) {
        tokens = Utils::tokenize(clauses[i], special_chars);
        curr_syns = QueryParser::parseSynonyms(tokens);
        all_syns.insert(all_syns.end(), curr_syns.begin(), curr_syns.end());
    }
    if (isDuplicateSynonymName(all_syns)) {
        throw SemanticError("Duplicate synonym names declared");
    }
    return Declaration(all_syns);
}

SelectClause QueryParser::parseSelectClause(std::string *clause,
                                            std::vector<Synonym> syns) {
    if (!std::regex_search(*clause, std::regex("\\b(Select)\\b"))) {
        throw SyntaxError("Expected select clause");
    }
    std::smatch matches;
    std::regex_match(*clause, matches, selectClauseRegex);
    std::string selectClause = matches[1];
    *clause = Utils::removeString(*clause, selectClause);
    std::regex_match(selectClause, matches, selectRegex);
    if (matches.size() == 0) {
        throw SyntaxError("Invalid select clause syntax");
    }

    std::string selectValue = Utils::removeTrailingSpaces(matches[1]);
    std::vector<Reference> selectedRefs;
    if (selectValue.compare("BOOLEAN") == 0) {
        return SelectClause(selectedRefs, SelectType::BOOLEAN);
    } else if (std::regex_search(selectValue, selectTupleRegex)) {
        selectValue = selectValue.substr(1, selectValue.size() - 2);
        std::vector<std::string> selectStrings =
            Utils::splitString(selectValue, ',');
        for (int i = 0; i < selectStrings.size(); i++) {
            std::string value = Utils::removeTrailingSpaces(selectStrings[i]);
            if (!std::regex_search(value, attrRefRegex) &&
                !std::regex_search(value, synRegex)) {
                throw SyntaxError("Invalid select value");
            }
            Reference selectedRef = QueryParser::getReference(value, syns);
            selectedRefs.push_back(selectedRef);
        }
        return SelectClause(selectedRefs, SelectType::TUPLE);
    } else if (std::regex_search(selectValue, attrRefRegex) ||
               std::regex_search(selectValue, synRegex)) {
        Reference selectedRef = QueryParser::getReference(selectValue, syns);
        selectedRefs.push_back(selectedRef);
        return SelectClause(selectedRefs, SelectType::SINGLE);
    }
}

void QueryParser::parseSuchThatClause(
    std::string *clause, std::vector<Synonym> syns,
    std::vector<SuchThatClause> *suchThatCls) {
    while (std::regex_search(
        *clause, std::regex("^\\s*such\\s+that\\s+|^\\s*and\\s+"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, suchThatClauseRegex);
        std::string suchThatClause = matches[1];
        std::regex_match(suchThatClause, matches, suchThatRegex);
        if (matches.size() != 5) {
            throw SyntaxError("Invalid such that clause syntax");
        }

        // throw an error if unable to find the relationship from the enum table
        if (relationshipMap.find(matches[2]) == relationshipMap.end()) {
            throw SyntaxError("Invalid relationship type");
        }
        RelationshipReference relationship = relationshipMap.at(matches[2]);
        Reference left = getReference(matches[3], syns);
        Reference right = getReference(matches[4], syns);
        if (!isValidSuchThatClause(relationship, left, right)) {
            throw SemanticError("Invalid relationship arguments");
        }

        *clause = Utils::removeString(*clause, suchThatClause);
        suchThatCls->push_back(SuchThatClause(relationship, left, right));
    }
}

void QueryParser::parsePatternClause(std::string *clause,
                                     std::vector<Synonym> syns,
                                     std::vector<PatternClause> *patternCls) {

    while (std::regex_search(*clause,
                             std::regex("^\\s*pattern\\s+|^\\s*and\\s+"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, patternClauseRegex);
        std::string patternClause = matches[1];
        std::regex_match(patternClause, matches, patternRegex);
        if (matches.size() != 7) {
            throw SyntaxError("Invalid pattern clause syntax");
        }
        Synonym syn = getSynonym(matches[2], syns);
        Reference entRef = getReference(matches[3], syns);
        Expression expr = Utils::removeTrailingSpaces(matches[5]);
        bool isExact = expr.find('_') == std::string::npos;
        if (!isValidPatternClause(syn, entRef, expr)) {
            throw SemanticError("Invalid pattern clause arguments");
        }
        if (expr.find('_') != std::string::npos && expr != "_") {
            // Remove _ at the start and end
            expr = expr.substr(1, expr.size() - 2);
            expr = Utils::removeTrailingSpaces(expr);
        }
        if (expr != "_") {
            if (syn.getEntityName() == EntityName::WHILE ||
                syn.getEntityName() == EntityName::IF) {
                throw SyntaxError("Invalid pattern second argument");
            }
            try {
                // Remove " at the start and end
                expr = expr.substr(1, expr.size() - 2);
                expr = Utils::removeTrailingSpaces(expr);
                expr = SP::convertExpression(expr);
            } catch (...) {
                throw SyntaxError("Invalid expression syntax");
            }
        }

        *clause = Utils::removeString(*clause, patternClause);
        patternCls->push_back(PatternClause(syn, entRef, expr, isExact));
    }
}

void QueryParser::parseWithClause(std::string *clause,
                                  std::vector<Synonym> syns,
                                  std::vector<WithClause> *withCls) {
    while (
        std::regex_search(*clause, std::regex("^\\s*with\\s+|^\\s*and\\s+"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, withClauseRegex);
        std::string withClause = matches[1];

        std::regex_match(withClause, matches, withRegex);
        if (matches.size() != 4) {
            throw SyntaxError("Invalid with clause syntax");
        }

        Reference left = getReference(matches[2], syns);
        Reference right = getReference(matches[3], syns);
        if (left.getRefType() == ReferenceType::WILDCARD ||
            right.getRefType() == ReferenceType::WILDCARD) {
            throw SyntaxError("With clause arguments cannot be wildcards");
        }
        if (!isValidWithClause(left, right)) {
            throw SemanticError("Invalid with clause arguments");
        }

        *clause = Utils::removeString(*clause, withClause);
        withCls->push_back(WithClause(left, right));
    }
}

bool QueryParser::isSuchThatClause(std::string *clause) {
    return std::regex_search(*clause, std::regex("^\\s*such\\s+that\\s+"));
}

bool QueryParser::isPatternClause(std::string *clause) {
    return std::regex_search(*clause, std::regex("^\\s*pattern\\s+"));
}

bool QueryParser::isWithClause(std::string *clause) {
    return std::regex_search(*clause, std::regex("^\\s*with\\s+"));
}

std::vector<Synonym>
QueryParser::parseSynonyms(std::vector<std::string> tokens) {
    if (tokens.size() % 2 == 1) {
        throw SyntaxError("syntax error in declaration clause");
    }
    EntityName entity;
    if (!entityMap.count(tokens[0])) {
        throw SyntaxError("Invalid design entity name");
    } else {
        entity = entityMap.find(tokens[0])->second;
    }
    std::vector<Synonym> syns;
    for (int i = 1; i < tokens.size(); i++) {
        if (i % 2 == 1) {
            if (!std::regex_match(tokens[i], synRegex)) {
                throw SyntaxError("Invalid name in declaration clause");
            }
            syns.push_back(Synonym(entity, tokens[i]));
        } else if (tokens[i].compare(",") != 0) {
            throw SyntaxError("Invalid declaration clause");
        }
    }
    return syns;
}

Reference QueryParser::getReference(std::string input,
                                    std::vector<Synonym> syns) {

    if (input[0] == '\"' && input.back() == '\"') {
        // Remove " at the start and end
        input = input.substr(1, input.size() - 2);
        input = Utils::removeTrailingSpaces(input);
        if (!std::regex_match(input, nameRegex)) {
            throw SyntaxError("Invalid reference format");
        }
        return Reference(input);
    }
    if (std::regex_match(input, intRegex) ||
        std::regex_match(input, wildcardRegex)) {
        return Reference(input);
    } else if (std::regex_match(input, synRegex)) {
        Synonym synonym = getSynonym(input, syns);
        return Reference(synonym);
    } else if (std::regex_match(input, attrRefRegex)) {
        std::smatch matches;
        std::regex_match(input, matches, attrRefRegex);
        Synonym synonym = getSynonym(matches[1], syns);
        if (!entityAttrMap.count(matches[2])) {
            throw SyntaxError("Invalid attribute name");
        }
        EntityAttribute attr = entityAttrMap.find(matches[2])->second;
        return Reference(synonym, attr);
    }
    throw SyntaxError("Invalid reference format");
}

Synonym QueryParser::getSynonym(std::string input, std::vector<Synonym> syns) {
    for (int i = 0; i < syns.size(); i++) {
        Synonym synonym = syns[i];
        if (input.compare(synonym.getName()) == 0) {
            return synonym;
        }
    }
    throw SemanticError("Synonym not found");
}

bool QueryParser::isValidSuchThatClause(RelationshipReference relRef,
                                        Reference left, Reference right) {
    if (relRef == RelationshipReference::EMPTY) {
        return true;
    }
    bool isLeftValid;
    bool isRightValid;
    std::unordered_set<EntityName> validLeftArg =
        relationshipLeftArgMap.find(relRef)->second;
    std::unordered_set<EntityName> validRightArg =
        relationshipRightArgMap.find(relRef)->second;
    std::unordered_set<ReferenceType> validLeftRef =
        relationshipLeftRefMap.find(relRef)->second;
    std::unordered_set<ReferenceType> validRightRef =
        relationshipRightRefMap.find(relRef)->second;

    if (left.isASynonym()) {
        isLeftValid = validLeftArg.count(left.getEntityName());
    } else {
        isLeftValid = validLeftRef.count(left.getRefType());
    }
    if (right.isASynonym()) {
        isRightValid = validRightArg.count(right.getEntityName());
    } else {
        isRightValid = validRightRef.count(right.getRefType());
    }
    return isLeftValid && isRightValid;
}

bool QueryParser::isValidPatternClause(Synonym syn, Reference entRef,
                                       Expression expr) {
    if (!patternEntityMap.count(syn.getEntityName())) {
        return false;
    } else if ((entRef.isASynonym() &&
                entRef.getEntityName() != EntityName::VARIABLE) ||
               entRef.getRefType() == ReferenceType::STMT_REF ||
               entRef.getRefType() == ReferenceType::ATTR_REF) {
        return false;
    }
    return true;
}

bool QueryParser::isValidWithClause(Reference left, Reference right) {
    if (left.isASynonym() || right.isASynonym()) {
        return false;
    }
    if (left.getAttr() == EntityAttribute::PROC_NAME ||
        left.getAttr() == EntityAttribute::VAR_NAME ||
        left.getRefType() == ReferenceType::ENT_REF) {
        return (right.getAttr() == EntityAttribute::PROC_NAME ||
                right.getAttr() == EntityAttribute::VAR_NAME ||
                right.getRefType() == ReferenceType::ENT_REF);
    } else if (left.getAttr() == EntityAttribute::STMT_NO ||
               left.getAttr() == EntityAttribute::VALUE ||
               left.getRefType() == ReferenceType::STMT_REF) {
        return (right.getAttr() == EntityAttribute::STMT_NO ||
                right.getAttr() == EntityAttribute::VALUE ||
                right.getRefType() == ReferenceType::STMT_REF);
    } else {
        return false;
    }
}

bool QueryParser::isDuplicateSynonymName(std::vector<Synonym> syns) {
    std::vector<std::string> names;
    int i, j;
    for (i = 0; i < syns.size(); i++) {
        names.push_back(syns[i].getName());
    }
    for (i = 0; i < syns.size(); i++) {
        for (j = 0; j < syns.size(); j++) {
            if (i == j) {
                continue;
            } else if (syns[i].getName() == names[j]) {
                return true;
            }
        }
    }
    return false;
}
