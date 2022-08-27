#include "QueryParser.h"
#include "Utils.h"
#include <vector>
#include <list>

using namespace std;

void QueryParser::parse(string query) {
    vector<string> clauses = Utils::splitString(query, ';');
    int size = clauses.size();

    Declaration decl;
    SelectClause selectCl;
    SuchThatClause suchThatCl;
    PatternClause patternCl;

    // Handle declaration
    vector<Synonym> syns;
    for (int i = 0; i < size - 1; i++) {
        string clause = Utils::removeTrailingSpaces(clauses[i]);
        syns.push_back(Synonym(clause));
    }
    decl = Declaration(syns);

    // Handle main clause
    string mainClause = Utils::removeTrailingSpaces(clauses[size - 1]);
    vector<string> tokens = Utils::splitString(mainClause, ' ');

    if (tokens[0].compare("Select") == 0) {
        Synonym selectedSyn;
        for (int i = 0; i < syns.size(); i++) {
            Synonym s = syns[i];
            if (s.name.compare(tokens[1]) == 0) {
                selectedSyn = s;
                break;
            }
        }
        selectCl = SelectClause(selectedSyn);
    }
    size_t suchThatIdx = mainClause.find("such that");
    size_t patternIdx = mainClause.find("pattern");
    string suchThatString;
    string patternString;
    if (suchThatIdx < mainClause.size())
        suchThatString = mainClause.substr(suchThatIdx, min(patternIdx, mainClause.size()) - suchThatIdx);

    if (patternIdx < mainClause.size())
        patternString = mainClause.substr(patternIdx);
}