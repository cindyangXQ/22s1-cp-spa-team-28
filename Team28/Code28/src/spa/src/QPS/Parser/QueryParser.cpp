#include "QueryParser.h"
#include "../../SP/SP.h"

SolvableQuery QueryParser::parse(std::string query) {
    if (query.back() == ';') {
        throw SyntaxError("Semicolon at the end not allowed");
    }
    std::vector<std::string> clauses = Utils::splitString(query, ';');
    size_t size = clauses.size();

    Declaration decl;
    SelectClause selectClause;
    std::vector<SuchThatClause> suchThatCls;
    std::vector<PatternClause> patternCls;
    std::vector<WithClause> withCls;

    if (size >= 2) {
        decl = QueryParser::parseDeclaration(clauses);
    }

    // Extract main clause
    std::string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
    selectClause = QueryParser::parseSelectClause(&mainClause, decl.syns);
    while (!Utils::removeTrailingSpaces(mainClause).empty()) {
        if (QueryParser::isSuchThatClause(&mainClause)) {
            QueryParser::parseSuchThatClause(&mainClause, decl.syns,
                                             &suchThatCls);
        } else if (QueryParser::isPatternClause(&mainClause)) {
            QueryParser::parsePatternClause(&mainClause, decl.syns, &patternCls);
        } 
        else if (QueryParser::isWithClause(&mainClause)) {
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
    if (std::regex_search(*clause, std::regex("\\b(Select)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, selectClauseRegex);
        std::string selectClause = matches[1];

        std::regex_match(selectClause, matches, selectRegex);
        if (matches.size() == 0) {
            throw SyntaxError("Invalid select clause syntax");
        }
        std::string selectValue = matches[1];
        std::vector<Reference> selectedRefs;
        *clause = Utils::removeString(*clause, selectClause);

        if (selectValue.compare("BOOLEAN") == 0) {
            return SelectClause(selectedRefs, SelectType::BOOLEAN);
        } else if (std::regex_search(selectValue, selectTupleRegex)) {
            selectValue = selectValue.substr(1, selectValue.size() - 2);
            std::vector<std::string> selectStrings =
                Utils::splitString(selectValue, ',');
            for (int i = 0; i < selectStrings.size(); i++) {
                std::string value = selectStrings[i];
                if (!std::regex_search(value, synRegex) && !std::regex_search(value, attrRefRegex)) {
                    throw SyntaxError("Invalid select value");
                }
                Reference selectedRef = QueryParser::getReference(
                    Utils::removeTrailingSpaces(value), syns);
                selectedRefs.push_back(selectedRef);
            }
            return SelectClause(selectedRefs, SelectType::TUPLE);
        } else if (std::regex_search(selectValue, attrRefRegex) || std::regex_search(selectValue, synRegex)) {
            selectValue = Utils::removeTrailingSpaces(selectValue);
            Reference selectedRef = QueryParser::getReference(selectValue, syns);
            selectedRefs.push_back(selectedRef);
            return SelectClause(selectedRefs, SelectType::SINGLE);
        }
    }
    throw SyntaxError("Expected select clause");
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
        if (relationshipMap.find(matches[2].str()) == relationshipMap.end()) {
            throw SyntaxError("Invalid relationship type");
        }
        RelationshipReference relationship =
            relationshipMap.at(matches[2].str());
        Reference left = getReference(matches[3].str(), syns);
        Reference right = getReference(matches[4].str(), syns);
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
        std::string patternClause = matches[1].str();

        std::regex_match(patternClause, matches, patternRegex);
        if (matches.size() != 7) {
            throw SyntaxError("Invalid pattern clause syntax");
        }

        Synonym syn = getSynonym(matches[2].str(), syns);
        if (!patternEntityMap.count(syn.entity)) {
            throw SemanticError(
                "Pattern only accepts assign, while and if synonym");
        }

        Reference entRef = getReference(matches[3].str(), syns);
        if (entRef.isSynonym && entRef.syn.entity != EntityName::VARIABLE) {
            throw SemanticError("Pattern first argument must be an entity "
                                "reference or wildcard");
        } else if (entRef.type == ReferenceType::STMT_REF) {
            throw SemanticError("Pattern first argument must be an entity "
                                "reference or wildcard");
        }
        Expression expr = matches[5].str();
        bool isExact = true;

        expr = Utils::removeTrailingSpaces(expr);
        if (expr.find('_') != std::string::npos) {
            isExact = false;
            if (expr.compare("_") != 0) {
                // Remove _ at the start and end
                expr.erase(std::remove(expr.begin(), expr.end(), '_'),
                           expr.end());
            }
        }
        if (expr.compare("_") != 0) {
            if (syn.entity == EntityName::WHILE ||
                syn.entity == EntityName::IF) {
                throw SyntaxError("Pattern-if and pattern-while second (and "
                                  "third) argument can only be a wildcard");
            }
            try {
                // Remove " at the start and end
                expr.erase(std::remove(expr.begin(), expr.end(), '"'),
                           expr.end());
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
    while (std::regex_search(*clause, std::regex("^\\s*with\\s+|^\\s*and\\s+"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, withClauseRegex);
        std::string withClause = matches[1].str();

        std::regex_match(withClause, matches, withRegex);
        if (matches.size() != 4) {
            throw SyntaxError("Invalid with clause syntax");
        }

        Reference left = getReference(matches[2].str(), syns);
        Reference right = getReference(matches[3].str(), syns);
        if (left.isSynonym || right.isSynonym) {
            throw SemanticError("Invalid with clause arguments");
        }
        
        *clause = Utils::removeString(*clause, withClause);
        withCls->push_back(WithClause(left, right));
    }
}

bool QueryParser::isSuchThatClause(std::string* clause) {
    return std::regex_search(*clause, std::regex("^\\s*such\\s+that\\s+"));
}

bool QueryParser::isPatternClause(std::string *clause) {
    return std::regex_search(*clause, std::regex("^\\s*pattern\\s+"));
}

bool QueryParser::isWithClause(std::string *clause) {
    return std::regex_search(*clause, std::regex("^\\s*with\\s+"));
}

std::vector<Synonym> QueryParser::parseSynonyms(std::vector<std::string> tokens) {
    if (tokens.size() % 2 == 1) {
        throw SyntaxError("syntax error in declaration clause");
    }
    EntityName entity;
    if (entityMap.find(tokens[0]) == entityMap.end()) {
        throw SyntaxError("Invalid design entity name");
    } else {
        entity = entityMap.find(tokens[0])->second;
    }
    std::vector<Synonym> syns;
    for (int i = 1; i < tokens.size(); i++) {
        if (i % 2 == 1) {
            if (!isValidName(tokens[i])) {
                throw SyntaxError("Invalid name in declaration clause");
            }
            syns.push_back(Synonym(entity, tokens[i]));
        } else if (tokens[i].compare(",") != 0) {
            throw SyntaxError("syntax error in declaration clause");
        }
    }
    return syns;
}

Reference QueryParser::getReference(std::string input,
                                    std::vector<Synonym> syns) {
    if (std::all_of(input.begin(), input.end(), ::isdigit)) {
        return Reference(input);
    } else if (input[0] == '\"' && input.back() == '\"') {
        input.erase(std::remove(input.begin(), input.end(), '\"'), input.end());
        input = Utils::removeTrailingSpaces(input); 
        if (!std::regex_match(input, nameRegex)) {
            throw SyntaxError("Invalid reference format");
        }
        return Reference(input);
    } else if (input == "_") {
        return Reference(input);
    } else if (std::regex_match(input, synRegex)) {
        Synonym synonym = getSynonym(input, syns);
        return Reference(synonym);
    }
    else if (std::regex_match(input, attrRefRegex)) {
        std::smatch matches;
        std::regex_match(input, matches, attrRefRegex);
        Synonym synonym = getSynonym(matches[1].str(), syns);
        if (!entityAttrMap.count(matches[2])) {
            throw SyntaxError("Invalid attribute name");
        }
        EntityAttribute attr = entityAttrMap.find(matches[2].str())->second;
        return Reference(synonym, attr);
    }
    throw SyntaxError("Invalid reference format");
}

Synonym QueryParser::getSynonym(std::string input, std::vector<Synonym> syns) {
    for (int i = 0; i < syns.size(); i++) {
        Synonym synonym = syns[i];
        if (input.compare(synonym.name) == 0) {
            return synonym;
        }
    }
    throw SemanticError("Synonym not found");
}

bool QueryParser::isValidName(std::string name) {
    return std::regex_match(name, std::regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool QueryParser::isValidSuchThatClause(RelationshipReference relRef,
                                        Reference left, Reference right) {
    if (relRef == RelationshipReference::EMPTY) {
        return true;
    }
    bool isLeftValid;
    bool isRightValid;
    if (left.isSynonym) {
        isLeftValid =
            relationshipLeftArgMap.find(relRef)->second.count(left.syn.entity);
    } else {
        isLeftValid =
            relationshipLeftRefMap.find(relRef)->second.count(left.type);
    }
    if (right.isSynonym) {
        isRightValid = relationshipRightArgMap.find(relRef)->second.count(
            right.syn.entity);
    } else {
        isRightValid =
            relationshipRightRefMap.find(relRef)->second.count(right.type);
    }
    return isLeftValid && isRightValid;
}

bool QueryParser::isDuplicateSynonymName(std::vector<Synonym> syns) {
    std::vector<std::string> names;
    int i, j;
    for (i = 0; i < syns.size(); i++) {
        names.push_back(syns[i].name);
    }
    for (i = 0; i < syns.size(); i++) {
        for (j = 0; j < syns.size(); j++) {
            if (i == j) {
                continue;
            } else if (syns[i].name == names[j]) {
                return true;
            }
        }
    }
    return false;
}
