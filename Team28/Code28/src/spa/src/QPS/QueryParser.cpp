#include "QueryParser.h"
#include "Utils.h"
#include "./Error/ParseError.h"

SolvableQuery QueryParser::parse(std::string query) {
    std::vector<std::string> clauses = Utils::splitString(query, ';');
    int size = clauses.size();

    // Extract main clause
    std::string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
    if (size < 2) {
        throw ParseError("Expected declaration at the start of query");
    }

    Declaration decl = QueryParser::parseDeclaration(clauses);
    SelectClause selectCl = QueryParser::parseSelectClause(mainClause, decl.syns);
    SuchThatClause suchThatCl = QueryParser::parseSuchThatClause(mainClause);
    PatternClause patternCl = QueryParser::parsePatternClause(mainClause);

    return SolvableQuery(decl, selectCl, suchThatCl, patternCl);
}

Declaration QueryParser::parseDeclaration(std::vector<std::string> clauses) {
    std::vector<Synonym> syns;
    for (int i = 0; i < clauses.size() - 1; i++) {
        std::string clause = Utils::removeTrailingSpaces(clauses[i]);
        syns.push_back(QueryParser::parseSynonym(clause));
    }
    return Declaration(syns);
}

Synonym QueryParser::parseSynonym(std::string desc) {
    std::vector<std::string> tokens = Utils::splitString(desc, ' ');
    DesignEntity entity;
    if (tokens[0].compare("Procedure") == 0) {
        entity = DesignEntity::PROCEDURE;
    }
    else if (tokens[0].compare("Statement") == 0) {
        entity = DesignEntity::STATEMENT;
    }
    else if (tokens[0].compare("Read") == 0) {
        entity = DesignEntity::READ;
    }
    else if (tokens[0].compare("Print") == 0) {
        entity = DesignEntity::PRINT;
    }
    else if (tokens[0].compare("Assign") == 0) {
        entity = DesignEntity::ASSIGN;
    }
    else if (tokens[0].compare("Call") == 0) {
        entity = DesignEntity::CALL;
    }
    else if (tokens[0].compare("While") == 0) {
        entity = DesignEntity::WHILE;
    }
    else if (tokens[0].compare("If") == 0) {
        entity = DesignEntity::IF;
    }
    else if (tokens[0].compare("Variable") == 0) {
        entity = DesignEntity::VARIABLE;
    }
    else if (tokens[0].compare("Constant") == 0) {
        entity = DesignEntity::CONSTANT;
    }
    std::string name = tokens[1];
    return Synonym(entity, name);
}

SelectClause QueryParser::parseSelectClause(std::string mainClause, std::vector<Synonym> syns) {
    std::vector<std::string> tokens = Utils::splitString(mainClause, ' ');
    if (tokens[0].compare("Select") == 0) {
        Synonym selectedSyn;
        bool isSynInit = false;
        for (int i = 0; i < syns.size(); i++) {
            Synonym s = syns[i];
            if (s.name.compare(tokens[1]) == 0) {
                selectedSyn = s;
                isSynInit = true;
                break;
            }
        }
        if (isSynInit == false ) {
            throw ParseError("Synonym not initialized");
        }
        return SelectClause(selectedSyn);
    }
    else {
        throw ParseError("Expected select clause at the start of main clause");
    }
}

SuchThatClause QueryParser::parseSuchThatClause(std::string mainClause) {
    return SuchThatClause();
    /*
    size_t suchThatIdx = mainClause.find("such that");
    size_t patternIdx = mainClause.find("pattern");
    if (suchThatIdx < mainClause.size()) {
        std::string suchThatString = mainClause.substr(suchThatIdx, std::min(patternIdx, mainClause.size()) - suchThatIdx);
        return SuchThatClause(suchThatString);
    }
    else {
        return SuchThatClause();
    }
    */
}

PatternClause QueryParser::parsePatternClause(std::string mainClause) {
    return PatternClause();
    /*
    size_t suchThatIdx = mainClause.find("such that");
    size_t patternIdx = mainClause.find("pattern");
    if (patternIdx < mainClause.size()) {
        std::string patternString = mainClause.substr(patternIdx);
        return PatternClause(patternString);
    }
    else {
        return PatternClause();
    }
    */
}

