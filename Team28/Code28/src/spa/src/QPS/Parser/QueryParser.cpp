#include "QueryParser.h"

SolvableQuery QueryParser::parse(std::string query) {
    std::vector<std::string> clauses = Utils::splitString(query, ';');
    size_t size = clauses.size();

    Declaration decl;
    SelectType selectType;
    SuchThatClause suchThatCl;
    PatternClause patternCl;

    if (size >= 2) {
        decl = QueryParser::parseDeclaration(clauses);
    }

    // Extract main clause
    std::string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
    selectType = QueryParser::parseSelectClause(&mainClause, decl.syns);
    suchThatCl = QueryParser::parseSuchThatClause(&mainClause, decl.syns);
    patternCl = QueryParser::parsePatternClause(&mainClause, decl.syns);

    if (!mainClause.empty()) {
        throw SyntaxError("Unrecognized clause syntax");
    }

    return SolvableQuery(decl, selectType, suchThatCl, patternCl);
}

Declaration QueryParser::parseDeclaration(std::vector<std::string> clauses) {
    std::vector<Synonym> all_syns;
    std::vector<Synonym> curr_syns;
    std::vector<std::string> tokens;
    std::vector<char> special_chars{ ','};
    for (int i = 0; i < clauses.size() - 1; i++) {
        tokens = Utils::tokenize(clauses[i], special_chars);
        curr_syns = QueryParser::parseSynonyms(tokens);
        all_syns.insert(all_syns.end(), curr_syns.begin(), curr_syns.end());
    }
    if (isDuplicateSynonymName(all_syns)) {
        throw SyntaxError("Duplicate synonym names declared");
    }
    return Declaration(all_syns);
}

SelectType QueryParser::parseSelectClause(std::string *clause, std::vector<Synonym> syns) {
    if (std::regex_search(*clause, std::regex("\\b(Select)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches,selectClauseRegex);
        std::string selectClause = matches[1];

        std::regex_match(selectClause, matches,selectRegex);
        if (matches.size() == 0) {
            throw SyntaxError("Invalid select clause syntax");
        }
        SelectType selectType = QueryParser::getSynonym(matches[1], syns);

        *clause = Utils::removeString(*clause, selectClause);

        return selectType;
    }
    throw SyntaxError("Expected select clause");
}

SuchThatClause QueryParser::parseSuchThatClause(std::string *clause, std::vector<Synonym> syns) {
    if ((*clause).size() == 0) return SuchThatClause();
    if (std::regex_search(*clause, std::regex("\\b(such\\s+that)\\b"))) {
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
        RelationshipReference relationship = relationshipMap.at(matches[1].str());
        Reference left = getReference(matches[2].str(), syns);
        Reference right = getReference(matches[3].str(), syns);
        if (!isValidSuchThatClause(relationship, left, right)) {
            throw SemanticError("Invalid relationship arguments");
        }

        *clause = Utils::removeString(*clause, suchThatClause);

        return SuchThatClause(relationship, left, right);
    }
}

PatternClause QueryParser::parsePatternClause(std::string* clause, std::vector<Synonym> syns) {
    if ((*clause).size() == 0) return PatternClause();
    if (std::regex_search(*clause, std::regex("\\b(pattern)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, patternClauseRegex);
        std::string patternClause = matches[1];

        std::regex_match(patternClause, matches, patternRegex);
        if (matches.size() != 5) {
            throw SyntaxError("Invalid pattern clause syntax");
        }

        Synonym syn = getSynonym(matches[1].str(), syns);
        Reference entRef = getReference(matches[2].str(), syns);
        Expression expression = matches[4].str();

        *clause = Utils::removeString(*clause, patternClause);

        return PatternClause(syn, entRef, expression);

    }
}

std::vector<Synonym> QueryParser::parseSynonyms(std::vector<std::string> tokens) {
    if (tokens.size() % 2 == 1) {
        throw SyntaxError("syntax error in declaration clause");
    }
    EntityName entity;
    if (entityMap.find(tokens[0]) == entityMap.end()) {
        throw SyntaxError("Invalid design entity name");
    }
    else {
        entity = entityMap.find(tokens[0])->second;
    }
    std::vector<Synonym> syns;
    for (int i = 1; i < tokens.size(); i++) {
        if (i % 2 == 1) {
            if (!isValidName(tokens[i])) {
                throw SyntaxError("Invalid name in declaration clause");
            }
            syns.push_back(Synonym(entity, tokens[i]));
        }
        else if (tokens[i].compare(",") != 0) {
            throw SyntaxError("syntax error in declaration clause");
        }
    }
    return syns;
}

Reference QueryParser::getReference(std::string input, std::vector<Synonym> syns) {
    if (std::all_of(input.begin(), input.end(), ::isdigit)) {
        return Reference(input.c_str());
    }
    else if (input[0] == '\"' && input.back() == '\"') {
        return Reference(input.substr(1, input.size() - 2));
    }
    else if (input == "_") {
        return Reference(input);
    }    
    for (int i = 0; i < syns.size(); i++) {
        Synonym synonym = syns[i];
        if (input.compare(synonym.name) == 0) {
            return Reference(synonym);
        }
    }
    throw SyntaxError("Not a number, not a name, not a synonym declared");
}

Synonym QueryParser::getSynonym(std::string input, std::vector<Synonym> syns) {
    for (int i = 0; i < syns.size(); i++) {
        Synonym synonym = syns[i];
        if (input.compare(synonym.name) == 0) {
            return synonym;
        }
    }
    throw SyntaxError("Synonym not found");
}

bool QueryParser::isValidName(std::string name) {
    return std::regex_match(name, std::regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

bool QueryParser::isValidSuchThatClause(RelationshipReference relRef, Reference left, Reference right) {
    if (relRef == RelationshipReference::EMPTY) {
        return true;
    }
    else if (relRef == RelationshipReference::FOLLOWS 
        || relRef == RelationshipReference::FOLLOWS_T 
        || relRef == RelationshipReference::PARENT 
        || relRef == RelationshipReference::PARENT_T) {
        return (left.type != ReferenceType::ENT_REF && right.type != ReferenceType::ENT_REF);
    }
    else if (relRef == RelationshipReference::USES || relRef == RelationshipReference::MODIFIES) {
        if (left.type == ReferenceType::WILDCARD) {
            return false;
        }
        return (right.type != ReferenceType::STMT_REF);
    }
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
            }
            else if (syns[i].name == names[j]) {
                return true;
            }
        }
    }
    return false;
}