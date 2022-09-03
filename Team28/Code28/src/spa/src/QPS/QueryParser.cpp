#include "QueryParser.h"

SolvableQuery QueryParser::parse(std::string query) {
    std::vector<std::string> clauses = Utils::splitString(query, ';');
    size_t size = clauses.size();

    // Extract main clause
    std::string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
    if (size < 2) {
        throw ParseError("Expected declaration at the start of query");
    }

    Declaration decl = QueryParser::parseDeclaration(clauses);
    SelectClause selectCl = QueryParser::parseSelectClause(mainClause, decl.syns);
    SuchThatClause suchThatCl = QueryParser::parseSuchThatClause(mainClause, decl.syns);
    PatternClause patternCl = QueryParser::parsePatternClause(mainClause, decl.syns);

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

SuchThatClause QueryParser::parseSuchThatClause(std::string mainClause, std::vector<Synonym> syns) {
    std::vector<char> special_char{ ';', '(', ',', ')', '_' };
    std::vector<std::string> tokens = Utils::tokenize(mainClause, special_char);
    size_t i;
    for (i = 0; i < tokens.size(); i++) {
        if (isSuchThatClause(tokens, i)) {
            RelationshipReference r = getRelationshipReference(tokens[i]);
            Reference left = getReference(tokens[i + 4], syns);
            Reference right = getReference(tokens[i + 6], syns);
            return SuchThatClause(r, left, right);
        }
    }
    return SuchThatClause();
}

PatternClause QueryParser::parsePatternClause(std::string mainClause, std::vector<Synonym> syns) {
    std::vector<char> special_char{ ';', '(', ',', ')', '_' };
    std::vector<std::string> tokens = Utils::tokenize(mainClause, special_char);
    size_t i;
    for (i = 0; i < tokens.size(); i++) {
        if (isPatternClause(tokens, i)) {
            Synonym syn = getSynonym(tokens[i + 1], syns);
            Reference entRef = getReference(tokens[i + 3], syns);
            Expression expression = tokens[i + 5];
            return PatternClause(syn, entRef, expression);
        }
    }
    return PatternClause();
}

bool QueryParser::isSuchThatClause(std::vector<std::string> tokens, size_t start) {
    if (start > tokens.size() - 8) {
        return false;
    }
    std::vector<std::string> relref{ "Modifies", "Uses", "Parent", "Parent*", "Follows", "Follows*" };
    if (tokens[start] == "such" && tokens[start + 1] == "that") {
        if (Utils::in(relref, tokens[start + 2]) &&
            tokens[start + 3] == "(" &&
            tokens[start + 5] == "," &&
            tokens[start + 7] == ")") {
            return true;
        }
        else {
            throw ParseError("Syntax error for such that clause");
        }
    }
    return false;
}

bool QueryParser::isPatternClause(std::vector<std::string> tokens, size_t start) {
    if (tokens[start] == "pattern") {
        if (tokens[start + 2] == "(" &&
            tokens[start + 4] == "," &&
            tokens[tokens.size() - 8] == ")") {
            return true;
        }
        else {
            throw ParseError("Syntax error for pattern clause");
        }
    }
    return false;
}

RelationshipReference QueryParser::getRelationshipReference(std::string input) {
    RelationshipReference r = RelationshipReference::Empty;
    if (input.compare("Modifies") == 0) {
        r = RelationshipReference::Modifies;
    }
    else if (input.compare("Uses") == 0) {
        r = RelationshipReference::Uses;
    }
    else if (input.compare("Parent") == 0) {
        r = RelationshipReference::Parent;
    }
    else if (input.compare("Parent*") == 0) {
        r = RelationshipReference::ParentT;
    }
    else if (input.compare("Follows") == 0) {
        r = RelationshipReference::Follows;
    }
    else if (input.compare("Follows*") == 0) {
        r = RelationshipReference::FollowsT;
    }
    return r;
}

Reference QueryParser::getReference(std::string input, std::vector<Synonym> syns) {
    if (std::all_of(input.begin(), input.end(), ::isdigit)) {
        return StatementNumber(atoi(input.c_str()));
    }
    if (input[0] == '"' && input[-1] == '"') {
        return VariableName(input);
    }
    Synonym selectedSyn;
    for (int i = 0; i < syns.size(); i++) {
        Synonym s = syns[i];
        if (input.compare(s.name) == 0) {
            selectedSyn = s;
            return selectedSyn;
        }
    }
    throw ParseError("not a number, not a name, not a synonym declared");
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
    throw ParseError("not a number, not a name, not a synonym declared");
}