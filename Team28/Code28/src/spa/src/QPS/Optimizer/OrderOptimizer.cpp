#include "OrderOptimizer.h"

void OrderOptimizer::optimize(SolvableQuery *solvableQ) {
    std::vector<std::vector<QueryClause *>> clauseGroups =
        solvableQ->getClauseGroup();
    std::vector<std::vector<QueryClause *>> sortedClauseGroups;
    std::vector<std::pair<int, double>> clauseGroupIdxToScore;

    for (int i = 0; i < clauseGroups.size(); i++) {
        std::vector<QueryClause *> group = clauseGroups[i];
        double totalScore = getTotalScore(group);
        sort(group.begin(), group.end(),
             [](QueryClause *a, QueryClause *b) -> bool {
                 return a->getOptimizeScore() < b->getOptimizeScore();
             });
        clauseGroups[i] = group;
        clauseGroupIdxToScore.push_back({i, totalScore});
    }

    sort(clauseGroupIdxToScore.begin(), clauseGroupIdxToScore.end(),
         [](std::pair<int, double> a, std::pair<int, double> b) -> bool {
             return a.second < b.second;
         });
    for (std::pair<int, double> mapping : clauseGroupIdxToScore) {
        sortedClauseGroups.push_back(clauseGroups[mapping.first]);
    }
    solvableQ->setClauseGroup(&sortedClauseGroups);
}

double OrderOptimizer::getTotalScore(std::vector<QueryClause *> group) {
    double totalScore = 0.0;
    for (QueryClause *clause : group) {
        totalScore += clause->getOptimizeScore();
    }
    return totalScore;
}