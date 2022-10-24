#include "QueryParser.h"
#include "../../SP/SP.h"

SolvableQuery QueryParser::parse(std::string query) {
    if (query.back() == CLAUSE_SEPARATOR) {
        throw SyntaxError("Semicolon at the end not allowed");
    }
    std::vector<std::string> clauses =
        Utils::splitString(query, CLAUSE_SEPARATOR);
    Declaration decl;
    SelectClause selectClause;
    std::vector<SuchThatClause> suchThatCls;
    std::vector<PatternClause> patternCls;
    std::vector<WithClause> withCls;

    if (clauses.size() >= MIN_CLAUSE_NUM) {
        decl = QueryParser::parseDeclaration(clauses);
    }

    // Extract main clause
    std::string mainClause = Utils::trimSpaces(clauses[clauses.size() - 1]);
    selectClause = QueryParser::parseSelectClause(&mainClause, decl.getSyns());
    while (!Utils::trimSpaces(mainClause).empty()) {
        if (QueryParser::isSuchThatClause(&mainClause)) {
            QueryParser::parseSuchThatClause(&mainClause, decl.getSyns(),
                                             &suchThatCls);
        } else if (QueryParser::isPatternClause(&mainClause)) {
            QueryParser::parsePatternClause(&mainClause, decl.getSyns(),
                                            &patternCls);
        } else if (QueryParser::isWithClause(&mainClause)) {
            QueryParser::parseWithClause(&mainClause, decl.getSyns(), &withCls);
        } else {
            throw SyntaxError("Unrecognized clause syntax");
        }
    }
    return SolvableQuery(decl, selectClause, suchThatCls, patternCls, withCls);
}

Declaration QueryParser::parseDeclaration(std::vector<std::string> clauses) {
    std::vector<Synonym> allSyns;
    std::vector<Synonym> currSyns;
    std::vector<std::string> tokens;
    std::vector<char> specialChars{ARG_SEPARATOR};

    for (int i = 0; i < clauses.size() - 1; i++) {
        tokens = Utils::tokenize(clauses[i], specialChars);
        currSyns = QueryParser::parseSynonyms(tokens);
        allSyns.insert(allSyns.end(), currSyns.begin(), currSyns.end());
    }
    if (isDuplicateSynonymName(allSyns)) {
        throw SemanticError("Duplicate synonym names declared");
    }
    return Declaration(allSyns);
}

SelectClause QueryParser::parseSelectClause(std::string *clause,
                                            std::vector<Synonym> syns) {
    if (!isSelectClause(clause)) {
        throw SyntaxError("Expected select clause");
    }
    std::smatch matches;
    std::regex_match(*clause, matches, SELECT_CL_REGEX);
    std::string selectClause = matches[1];
    *clause = Utils::removeString(*clause, selectClause);
    std::regex_match(selectClause, matches, SELECT_REGEX);
    if (matches.size() == 0) {
        throw SyntaxError("Invalid select clause syntax");
    }

    bool has_BOOLEAN_synonym = false;
    for (int i = 0; i < syns.size(); i++) {
        if (syns[i].getName() == "BOOLEAN") {
            has_BOOLEAN_synonym = true;
        }
    }

    std::string selectValue = Utils::trimSpaces(matches[1]);
    if (!has_BOOLEAN_synonym && std::regex_search(selectValue, SELECT_BOOL_REGEX)) {
        return SelectClause({}, SelectType::BOOLEAN);
    } else if (std::regex_search(selectValue, SELECT_TUP_REGEX)) {
        return parseSelectTuple(selectValue, syns);
    } else if (std::regex_search(selectValue, ATTR_REF_REGEX) ||
               std::regex_search(selectValue, SYN_REGEX)) {
        Reference selectedRef = QueryParser::getReference(selectValue, syns);
        return SelectClause({selectedRef}, SelectType::SINGLE);
    } else {
        throw SyntaxError("Invalid select clause syntax");
    }
}

SelectClause QueryParser::parseSelectTuple(std::string selectValue,
                                           std::vector<Synonym> syns) {
    std::vector<Reference> selectedRefs;
    selectValue = selectValue.substr(1, selectValue.size() - 2);
    std::vector<std::string> selectStrings =
        Utils::splitString(selectValue, ARG_SEPARATOR);
    for (int i = 0; i < selectStrings.size(); i++) {
        std::string value = Utils::trimSpaces(selectStrings[i]);
        if (!std::regex_search(value, ATTR_REF_REGEX) &&
            !std::regex_search(value, SYN_REGEX)) {
            throw SyntaxError("Invalid select value");
        }
        Reference selectedRef = QueryParser::getReference(value, syns);
        selectedRefs.push_back(selectedRef);
    }
    return SelectClause(selectedRefs, SelectType::TUPLE);
}

void QueryParser::parseSuchThatClause(
    std::string *clause, std::vector<Synonym> syns,
    std::vector<SuchThatClause> *suchThatCls) {
    while (std::regex_search(*clause, IS_SUCH_THAT_AND)) {
        std::smatch matches;
        std::regex_match(*clause, matches, SUCH_THAT_CL_REGEX);
        std::string suchThatClause = matches[1];
        std::regex_match(suchThatClause, matches, SUCH_THAT_REGEX);
        if (matches.size() != SUCH_THAT_REGEX_CHECK) {
            throw SyntaxError("Invalid such that clause syntax");
        }

        // throw an error if unable to find the relationship from the enum table
        if (RELATIONSHIP_MAP.find(matches[2]) == RELATIONSHIP_MAP.end()) {
            throw SyntaxError("Invalid relationship type");
        }
        RelationshipReference relationship = RELATIONSHIP_MAP.at(matches[2]);
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

    while (std::regex_search(*clause, IS_PATTERN_AND)) {
        std::smatch matches;
        std::regex_match(*clause, matches, PATTERN_CL_REGEX);
        std::string patternClause = matches[1];
        std::regex_match(patternClause, matches, PATTERN_REGEX);
        if (matches.size() != PATTERN_REGEX_CHECK) {
            throw SyntaxError("Invalid pattern clause syntax");
        }
        Synonym syn = getSynonym(matches[2], syns);
        Reference entRef = getReference(matches[3], syns);
        Expression expr = Utils::trimSpaces(matches[5]);
        bool isExact = expr.find('_') == std::string::npos;
        if (!isValidPatternClause(syn, entRef, expr)) {
            throw SemanticError("Invalid pattern clause arguments");
        }
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

        *clause = Utils::removeString(*clause, patternClause);
        patternCls->push_back(PatternClause(syn, entRef, expr, isExact));
    }
}

void QueryParser::parseWithClause(std::string *clause,
                                  std::vector<Synonym> syns,
                                  std::vector<WithClause> *withCls) {
    while (std::regex_search(*clause, IS_WITH_AND)) {
        std::smatch matches;
        std::regex_match(*clause, matches, WITH_CL_REGEX);
        std::string withClause = matches[1];

        std::regex_match(withClause, matches, WITH_REGEX);
        if (matches.size() != WITH_REGEX_CHECK) {
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

bool QueryParser::isSelectClause(std::string *clause) {
    return std::regex_search(*clause, IS_SELECT);
}

bool QueryParser::isSuchThatClause(std::string *clause) {
    return std::regex_search(*clause, IS_SUCH_THAT);
}

bool QueryParser::isPatternClause(std::string *clause) {
    return std::regex_search(*clause, IS_PATTERN);
}

bool QueryParser::isWithClause(std::string *clause) {
    return std::regex_search(*clause, IS_WITH);
}

std::vector<Synonym>
QueryParser::parseSynonyms(std::vector<std::string> tokens) {
    if (tokens.size() % 2 == 1) {
        throw SyntaxError("syntax error in declaration clause");
    }
    EntityName entity;
    if (!ENTITY_MAP.count(tokens[0])) {
        throw SyntaxError("Invalid design entity name");
    } else {
        entity = ENTITY_MAP.find(tokens[0])->second;
    }
    std::vector<Synonym> syns;
    for (int i = 1; i < tokens.size(); i++) {
        if (i % 2 == 1) {
            if (!std::regex_match(tokens[i], SYN_REGEX)) {
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
        input = Utils::trimSpaces(input);
        if (!std::regex_match(input, NAME_REGEX)) {
            throw SyntaxError("Invalid reference format");
        }
        return Reference(input);
    }
    if (std::regex_match(input, INT_REGEX) ||
        std::regex_match(input, WILDCARD_REGEX)) {
        return Reference(input);
    } else if (std::regex_match(input, SYN_REGEX)) {
        Synonym synonym = getSynonym(input, syns);
        return Reference(synonym);
    } else if (std::regex_match(input, ATTR_REF_REGEX)) {
        std::smatch matches;
        std::regex_match(input, matches, ATTR_REF_REGEX);
        Synonym synonym = getSynonym(matches[1], syns);
        if (!ENTITY_ATTR_MAP.count(matches[2])) {
            throw SyntaxError("Invalid attribute name");
        }
        EntityAttribute attr = ENTITY_ATTR_MAP.find(matches[2])->second;
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
        RELATIONSHIP_LEFT_ARG_MAP.find(relRef)->second;
    std::unordered_set<EntityName> validRightArg =
        RELATIONSHIP_RIGHT_ARG_MAP.find(relRef)->second;
    std::unordered_set<ReferenceType> validLeftRef =
        RELATIONSHIP_LEFT_REF_MAP.find(relRef)->second;
    std::unordered_set<ReferenceType> validRightRef =
        RELATIONSHIP_RIGHT_REF_MAP.find(relRef)->second;

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
