#include "QueryParser.h"
#include "Utils.h"
#include <vector>
#include <list>

using namespace std;

void QueryParser::parse(string query) {
    vector<string> clauses = Utils::splitString(query, ';');
    int size = clauses.size();
    Declaration decl;
    list<Synonym> syns;
    // Handle declaration
    for (int i = 0; i < size - 1; i++) {
        string clause = Utils::removeTrailingSpaces(clauses[i]);
        syns.push_back(Synonym(clause));
    }

    // Handle main clause
    string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
}