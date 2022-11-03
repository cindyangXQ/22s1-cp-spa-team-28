#include "GroupOptimizer.h"

void GroupOptimizer::optimize(SolvableQuery *solvableQ) {
    std::vector<QueryClause *> clauses = solvableQ->getQueryClause();
    std::vector<QueryClause *> withCls = solvableQ->getWithClause();
    clauses.insert(std::end(clauses), std::begin(withCls), std::end(withCls));

    UFDS ufds(clauses.size());
    std::unordered_set<int> clausesWithoutSyn;
    std::unordered_map<int, std::unordered_set<std::string>>
        clauseIdxToSynUsedMap;
    std::unordered_map<std::string, std::vector<int>> synToClauseIdxMap;

    for (int i = 0; i < clauses.size(); i++) {
        QueryClause *clause = clauses[i];
        std::unordered_set<std::string> synsUsed = clause->getSynonymsUsed();
        if (synsUsed.size() == 0) {
            clausesWithoutSyn.insert(i);
            continue;
        }
        clauseIdxToSynUsedMap[i] = synsUsed;
        for (std::string syn : synsUsed) {
            if (synToClauseIdxMap.count(syn)) {
                int groupClauseMember = synToClauseIdxMap.at(syn)[0];
                ufds.unionSet(groupClauseMember, i);
            }
            synToClauseIdxMap[syn].push_back(i);
        }
    }

    std::vector<std::vector<QueryClause *>> clauseGroups(
        ufds.numDisjointSets(), std::vector<QueryClause *>());
    std::vector<int> groupMapping(clauses.size(), -1);
    int curGroup = 0;

    for (auto &clauseIdx : clauseIdxToSynUsedMap) {
        int synGroup = ufds.findSet(clauseIdx.first);
        int clauseGroup = groupMapping[synGroup];
        if (clauseGroup == -1) {
            clauseGroup = curGroup;
            groupMapping[synGroup] = curGroup;
            curGroup++;
        }
        clauseGroups[clauseGroup].push_back(clauses[clauseIdx.first]);
    }

    if (clausesWithoutSyn.size() > 0) {
        clauseGroups.insert(clauseGroups.begin(), std::vector<QueryClause *>());
        for (int clauseIdx : clausesWithoutSyn) {
            clauseGroups[0].push_back(clauses[clauseIdx]);
        }
    }

    solvableQ->setClauseGroup(&clauseGroups);
}