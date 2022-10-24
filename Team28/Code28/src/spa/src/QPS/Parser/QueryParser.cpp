#include "QueryParser.h"

template <typename QueryClass> bool QueryParser::isClause(std::string *clause) {
    return std::regex_search(*clause,
                             IS_CLAUSE_MAP.at(typeid(QueryClass)));
};

template <typename QueryClass>
void QueryParser::parse(std::string *queryString, std::vector<Synonym> syns,
                        std::vector<QueryClause *> *clauses) {
    while (std::regex_search(
        *queryString, IS_CLAUSE_AND_MAP.at(typeid(QueryClass)))) {
        std::smatch matches;
        std::regex_match(*queryString, matches,
                         WHOLE_CLAUSE_MAP.at(typeid(QueryClass)));
        std::string extractedClause = matches[1];
        std::regex_match(extractedClause, matches,
                         ARG_CLAUSE_MAP.at(typeid(QueryClass)));
        if (matches.size() !=
            REGEX_CHECK_MAP.at(typeid(QueryClass))) {
            throw SyntaxError("Invalid clause syntax");
        }
        QueryClause *queryClause = new QueryClass;
        queryClause->parse(matches, syns);
        if (!queryClause->validate()) {
            throw SemanticError("Invalid clause semantic");
        }
        clauses->push_back(queryClause);
        *queryString = Utils::removeString(*queryString, extractedClause);
    }
}

SolvableQuery QueryParser::parse(std::string query) {
    if (query.back() == CLAUSE_SEPARATOR) {
        throw SyntaxError("Semicolon at the end not allowed");
    }
    std::vector<std::string> clauses =
        Utils::splitString(query, CLAUSE_SEPARATOR);
    Declaration decl;
    SelectClause selectClause;
    std::vector<QueryClause *> queryCls;

    if (clauses.size() >= MIN_CLAUSE_NUM) {
        decl = QueryParser::parseDeclaration(clauses);
    }

    // Extract main clause
    std::string mainClause = Utils::trimSpaces(clauses[clauses.size() - 1]);
    selectClause = QueryParser::parseSelectClause(&mainClause, decl.getSyns());
    while (!Utils::trimSpaces(mainClause).empty()) {
        if (isClause<SuchThatClause>(&mainClause)) {
            parse<SuchThatClause>(&mainClause, decl.getSyns(), &queryCls);
        } else if (isClause<PatternClause>(&mainClause)) {
            parse<PatternClause>(&mainClause, decl.getSyns(), &queryCls);
        } else if (isClause<WithClause>(&mainClause)) {
            parse<WithClause>(&mainClause, decl.getSyns(), &queryCls);
        } else {
            throw SyntaxError("Unrecognized clause syntax");
        }
    }
    return SolvableQuery(decl, selectClause, queryCls);
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
    if (Synonym::isDuplicateSynonymName(allSyns)) {
        throw SemanticError("Duplicate synonym names declared");
    }
    return Declaration(allSyns);
}

SelectClause QueryParser::parseSelectClause(std::string *clause,
                                            std::vector<Synonym> syns) {
    if (!isClause<SelectClause>(clause)) {
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

    std::string selectValue = Utils::trimSpaces(matches[1]);
    if (std::regex_search(selectValue, SELECT_BOOL_REGEX)) {
        return SelectClause({}, SelectType::BOOLEAN);
    } else if (std::regex_search(selectValue, SELECT_TUP_REGEX)) {
        return parseSelectTuple(selectValue, syns);
    } else if (std::regex_search(selectValue, ATTR_REF_REGEX) ||
               std::regex_search(selectValue, SYN_REGEX)) {
        Reference selectedRef = Reference::getReference(selectValue, syns);
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
        Reference selectedRef = Reference::getReference(value, syns);
        selectedRefs.push_back(selectedRef);
    }
    return SelectClause(selectedRefs, SelectType::TUPLE);
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
        entity = ENTITY_MAP.at(tokens[0]);
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