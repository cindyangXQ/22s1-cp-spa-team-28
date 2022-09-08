#include "QueryParser.h"

SolvableQuery QueryParser::parse(std::string query) {
    std::vector<std::string> clauses = Utils::splitString(query, ';');
    size_t size = clauses.size();

    Declaration decl;
    SelectType selectType;
    SuchThatClause suchThatCl = SuchThatClause();
    PatternClause patternCl = PatternClause();

    if (size >= 2) {
        decl = QueryParser::parseDeclaration(clauses);
    }

    // Extract main clause
    std::string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
    selectType = QueryParser::parseSelectClause(&mainClause, decl.syns);
    suchThatCl = QueryParser::parseSuchThatClause(&mainClause, decl.syns);
    patternCl = QueryParser::parsePatternClause(&mainClause, decl.syns);

    return SolvableQuery(decl, selectType, suchThatCl, patternCl);
}

Declaration QueryParser::parseDeclaration(std::vector<std::string> clauses) {
    std::vector<Synonym> syns;
    for (int i = 0; i < clauses.size() - 1; i++) {
        std::string clause = Utils::removeTrailingSpaces(clauses[i]);
        if (std::regex_match(clause, declarationRegex)) {
            syns.push_back(QueryParser::parseSynonym(clause));
        }
        else {
            throw ParseError("Invalid declaration syntax");
        }
    }
    return Declaration(syns);
}

SelectType QueryParser::parseSelectClause(std::string *clause, std::vector<Synonym> syns) {
    if (std::regex_search(*clause, std::regex("\\b(Select)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches,selectClauseRegex);
        std::string selectClause = matches[1];

        std::regex_match(selectClause, matches,selectRegex);
        if (matches.size() == 0) {
            throw ParseError("Invalid select clause syntax");
        }
        SelectType selectType = QueryParser::getSynonym(matches[1], syns);

        *clause = Utils::removeString(*clause, selectClause);

        return selectType;
    }
    throw ParseError("Expected select clause");
}

SuchThatClause QueryParser::parseSuchThatClause(std::string *clause, std::vector<Synonym> syns) {
    if ((*clause).size() == 0) return SuchThatClause();
    if (std::regex_search(*clause, std::regex("\\b(such\\s+that)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, suchThatClauseRegex);
        std::string suchThatClause = matches[1];

        std::regex_match(suchThatClause, matches, suchThatRegex);
        if (matches.size() != 4) {
            throw ParseError("Invalid such that clause syntax");
        }

        // throw an error if unable to find the relationship from the enum table
        if (relationshipNameLookup.find(matches[1].str()) == relationshipNameLookup.end()) {
            throw ParseError("Invalid relationship type");
        }
        Relationship relationship = relationshipNameLookup.at(matches[1].str());
        Reference left = getReference(matches[2].str(), syns);
        Reference right = getReference(matches[3].str(), syns);

        *clause = Utils::removeString(*clause, suchThatClause);

        return SuchThatClause(relationship, left, right);
    }
    throw ParseError("Expected such that clause");
}

PatternClause QueryParser::parsePatternClause(std::string *clause, std::vector<Synonym> syns) {
    if ((*clause).size() == 0) return PatternClause();
    if (std::regex_search(*clause, std::regex("\\b(pattern)\\b"))) {
        std::smatch matches;
        std::regex_match(*clause, matches, patternClauseRegex);
        std::string patternClause = matches[1];

        std::regex_match(patternClause, matches,patternRegex);
        if (matches.size() != 5) {
            throw ParseError("Invalid pattern clause syntax");
        }

        Synonym syn = getSynonym(matches[1].str(), syns);
        Reference entRef = getReference(matches[2].str(), syns);
        Expression expression = matches[3].str();

        *clause = Utils::removeString(*clause, patternClause);

        return PatternClause(syn, entRef, expression);

    }
    throw ParseError("Expected pattern clause");
}

Synonym QueryParser::parseSynonym(std::string desc) {
    std::vector<std::string> tokens = Utils::splitString(desc, ' ');
    EntityName entity;
    if (entityNameLookup.find(tokens[0]) == entityNameLookup.end()) {
        throw ParseError("Invalid design entity name");
    }
    else {
        entity = entityNameLookup.find(tokens[0])->second;
    }
    std::string name = tokens[1];
    return Synonym(entity, name);
}

Reference QueryParser::getReference(std::string input, std::vector<Synonym> syns) {
    if (std::all_of(input.begin(), input.end(), ::isdigit)) {
        return Statement(atoi(input.c_str()));
    }
    if (input[0] == '"' && input[-1] == '"') {
        return Variable(input);
    }
    Synonym selectedSyn;
    for (int i = 0; i < syns.size(); i++) {
        Synonym s = syns[i];
        if (input.compare(s.name) == 0) {
            selectedSyn = s;
            return selectedSyn;
        }
    }
    throw ParseError("Not a number, not a name, not a synonym declared");
}

Synonym QueryParser::getSynonym(std::string input, std::vector<Synonym> syns) {
    Synonym selectedSyn;
    for (int i = 0; i < syns.size(); i++) {
        Synonym s = syns[i];
        if (input.compare(s.name) == 0) {
            selectedSyn = s;
            return selectedSyn;
        }
    }
    throw ParseError("Synonym not found");
}