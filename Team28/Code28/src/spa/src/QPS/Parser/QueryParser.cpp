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
    SelectType selectType = QueryParser::parseSelectClause(mainClause, decl.syns);
    SuchThatClause suchThatCl = QueryParser::parseSuchThatClause(mainClause, decl.syns);
    PatternClause patternCl = QueryParser::parsePatternClause(mainClause, decl.syns);

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
    if (checkDuplicateSynonymName(all_syns)) {
        throw ParseError("Duplicate synonym names declared");
    }
    return Declaration(all_syns);
}

std::vector<Synonym> QueryParser::parseSynonyms(std::vector<std::string> tokens) {
    if (tokens.size() % 2 == 1) {
        throw ParseError("syntax error in declaration clause");
    }
    DesignEntity entity = getDesignEntity(tokens[0]);
    std::vector<Synonym> syns;
    for (int i = 1; i < tokens.size(); i++) {
        if (i % 2 == 1) {
            if (!checkValidName(tokens[i])) {
                throw ParseError("Invalid name in declaration clause");
            }
            syns.push_back(Synonym(entity, tokens[i]));
        }
        else if (tokens[i].compare(",") != 0) {
            throw ParseError("syntax error in declaration clause");
        }
    }
    return syns;
}

SelectType QueryParser::parseSelectClause(std::string mainClause, std::vector<Synonym> syns) {
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
        return selectedSyn;
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

bool QueryParser::checkValidName(std::string name) {
    return std::regex_match(name, std::regex("^[a-zA-Z][a-zA-Z0-9]*$"));
}

DesignEntity QueryParser::getDesignEntity(std::string input) {
    DesignEntity entity;
    if (input.compare("procedure") == 0) {
        entity = DesignEntity::PROCEDURE;
    }
    else if (input.compare("stmt") == 0) {
        entity = DesignEntity::STATEMENT;
    }
    else if (input.compare("read") == 0) {
        entity = DesignEntity::READ;
    }
    else if (input.compare("print") == 0) {
        entity = DesignEntity::PRINT;
    }
    else if (input.compare("assign") == 0) {
        entity = DesignEntity::ASSIGN;
    }
    else if (input.compare("call") == 0) {
        entity = DesignEntity::CALL;
    }
    else if (input.compare("while") == 0) {
        entity = DesignEntity::WHILE;
    }
    else if (input.compare("if") == 0) {
        entity = DesignEntity::IF;
    }
    else if (input.compare("variable") == 0) {
        entity = DesignEntity::VARIABLE;
    }
    else if (input.compare("constant") == 0) {
        entity = DesignEntity::CONSTANT;
    }
    else {
        throw ParseError("Unknown design entity in declaration clause");
    }
    return entity;
}

bool QueryParser::checkDuplicateSynonymName(std::vector<Synonym> syns) {
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