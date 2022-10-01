#include "QueryParser.h"
#include "../../SP/SP.h"

SolvableQuery QueryParser::parse(std::string query) {
    if (query.back() == ';') {
        throw SyntaxError("Semicolon at the end not allowed");
    }
    std::vector<std::string> clauses = Utils::splitString(query, ';');
    size_t size = clauses.size();

    Declaration decl;
    SelectType selectType;
    std::vector<SuchThatClause> suchThatCls;
    std::vector<PatternClause> patternCls;

    if (size >= 2) {
        decl = QueryParser::parseDeclaration(clauses);
    }

    // Extract main clause
    std::string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
    selectType = QueryParser::parseSelectClause(&mainClause, decl.syns);
    suchThatCls = QueryParser::parseSuchThatClause(&mainClause, decl.syns);
    patternCls = QueryParser::parsePatternClause(&mainClause, decl.syns);

    if (!Utils::removeTrailingSpaces(mainClause).empty()) {
        throw SyntaxError("Unrecognized clause syntax");
    }

    return SolvableQuery(decl, selectType, suchThatCls, patternCls);
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

SelectType QueryParser::parseSelectClause(std::string *clause,
                                          std::vector<Synonym> syns) {
    if (std::regex_search(*clause, std::regex("\\b(Select)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, selectClauseRegex);
        std::string selectClause = matches[1];

        std::regex_match(selectClause, matches, selectRegex);
        if (matches.size() == 0) {
            throw SyntaxError("Invalid select clause syntax");
        }
        SelectType selectType = QueryParser::getSynonym(matches[1], syns);

        *clause = Utils::removeString(*clause, selectClause);

        return selectType;
    }
    throw SyntaxError("Expected select clause");
}

std::vector<SuchThatClause>
QueryParser::parseSuchThatClause(std::string *clause,
                                                std::vector<Synonym> syns) {
    std::vector<SuchThatClause> clauses;
    if ((*clause).size() == 0)
        return clauses;
    while (std::regex_search(*clause, std::regex("\\b(such\\s+that)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, suchThatClauseRegex);
        std::string suchThatClause = matches[1];
        
        std::regex_match(suchThatClause, matches, suchThatRegex);
        if (matches.size() != 4) {
            throw SyntaxError("Invalid such that clause syntax");
        }

        // throw an error if unable to find the relationship from the enum table
        if (relationshipMap.find(matches[1].str()) == relationshipMap.end()) {
            throw SyntaxError("Invalid relationship type");
        }
        RelationshipReference relationship =
            relationshipMap.at(matches[1].str());
        Reference left = getReference(matches[2].str(), syns);
        Reference right = getReference(matches[3].str(), syns);
        if (!isValidSuchThatClause(relationship, left, right)) {
            throw SemanticError("Invalid relationship arguments");
        }

        *clause = Utils::removeString(*clause, suchThatClause);

        clauses.push_back(SuchThatClause(relationship, left, right));
    }
    return clauses;
    
}

std::vector<PatternClause> QueryParser::parsePatternClause(std::string *clause,
                                              std::vector<Synonym> syns) {
    std::vector<PatternClause> clauses;
    if ((*clause).size() == 0)
        return clauses;
    while (std::regex_search(*clause, std::regex("\\b(pattern)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, patternClauseRegex);
        std::string patternClause = matches[1];

        std::regex_match(patternClause, matches, patternRegex);
        if (matches.size() != 5) {
            throw SyntaxError("Invalid pattern clause syntax");
        }

        Synonym syn = getSynonym(matches[1].str(), syns);
        if (syn.entity != EntityName::ASSIGN) {
            throw SemanticError("Pattern only accepts assign synonym");
        }
        Reference entRef = getReference(matches[2].str(), syns);
        if (syn.entity == EntityName::ASSIGN) {
            if (entRef.isSynonym && entRef.syn.entity != EntityName::VARIABLE) {
                throw SemanticError("Pattern-assign first argument must be an entity reference or wildcard");
            }
            else if (entRef.type == ReferenceType::STMT_REF) {
                throw SemanticError("Pattern-assign first argument must be an entity reference or wildcard");
            }
        }
        Expression expr = matches[4].str();
        bool isExact = false;

        expr = Utils::removeTrailingSpaces(expr);
        if (expr.find('_') != std::string::npos) {
            isExact = false;
            if (expr.compare("_") != 0) {
                //Remove _ at the start and end
                expr.erase(std::remove(expr.begin(), expr.end(), '_'), expr.end());
            }
        }
        if (expr.compare("_") != 0) {
            try {
                //Remove " at the start and end
                expr.erase(std::remove(expr.begin(), expr.end(), '"'), expr.end());
                
                expr = SP::convertExpression(expr);
            }
            catch (std::runtime_error e) {
                throw SyntaxError("Invalid expression syntax");
            }
        }
        *clause = Utils::removeString(*clause, patternClause);

        clauses.push_back(PatternClause(syn, entRef, expr, isExact));
    }
    return clauses;
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
        return Reference(input.substr(1, input.size() - 2));
    } else if (input == "_") {
        return Reference(input);
    }
    else if (std::regex_match(input, synRegex)) {
        Synonym synonym = getSynonym(input, syns);
        return Reference(synonym);
    }
    throw SyntaxError("Invalid reference format");
}

Synonym QueryParser::getSynonym(std::string input, 
                                std::vector<Synonym> syns) {
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
        isLeftValid = relationshipLeftArgMap.find(relRef)->second.count(left.syn.entity);
    } else {
        isLeftValid = relationshipLeftRefMap.find(relRef)->second.count(left.type);
    }
    if (right.isSynonym) {
        isRightValid = relationshipRightArgMap.find(relRef)->second.count(right.syn.entity);
    } else {
        isRightValid = relationshipRightRefMap.find(relRef)->second.count(right.type);
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
