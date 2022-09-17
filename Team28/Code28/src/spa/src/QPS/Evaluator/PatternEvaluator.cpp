#include "PatternEvaluator.h"

std::vector<ClauseResult> PatternEvaluator::evaluate(PatternClause *patternCl) {
    if (patternCl->patternType == PatternType::EMPTY) {
        return { ClauseResult(false) };
    }
    else if (patternCl->patternType == PatternType::ASSIGN) {
        if (patternCl->entRef.isSynonym) {
            std::string expr = patternCl->expression;

            if (expr.find('_') != std::string::npos) {
                Reference expressionVar = Reference(expr.substr(1, expr.size() - 2));
                ClauseResult clauseResultModifies = ClauseResult(std::vector{ patternCl->syn, patternCl->entRef.syn });
                ClauseResult clauseResultUses = ClauseResult(std::vector{ patternCl->syn});
                std::vector< std::pair<Value, Value> > resultModifies = queryFacade->solveBoth(RelationshipReference::MODIFIES, patternCl->syn.entity, patternCl->entRef.syn.entity);
                std::vector<Value> resultUses = queryFacade->solveLeft(RelationshipReference::USES, expressionVar, patternCl->syn.entity);

                for (int i = 0; i < resultModifies.size(); i++) {
                    clauseResultModifies.insert(Tuple(std::vector{ resultModifies[i].first, resultModifies[i].second}));
                }
                for (int i = 0; i < resultUses.size(); i++) {
                    clauseResultUses.insert(Tuple(std::vector{ resultUses[i] }));
                }

                return { clauseResultModifies, clauseResultUses };
            }
            else {
                std::vector< std::pair<Value, Value> > result = queryFacade->getAssignAndVar(patternCl->expression);
                ClauseResult clauseResult = ClauseResult(std::vector{ patternCl->syn, patternCl->entRef.syn });
                for (int i = 0; i < result.size(); i++) {
                    clauseResult.insert(Tuple(std::vector{ result[i].first, result[i].second}));
                }
                return { clauseResult };
            }
        }
        else {
            std::string varName = patternCl->entRef.value.value;
            std::string expr = patternCl->expression;

            if (expr.find('_') != std::string::npos) {
                Reference var = Reference(varName);
                Reference expressionVar = Reference(expr.substr(1, expr.size() - 2));
                ClauseResult clauseResultModifies = ClauseResult(std::vector{ patternCl->syn });
                ClauseResult clauseResultUses = ClauseResult(std::vector{ patternCl->syn });
                std::vector<Value> resultModifies = queryFacade->solveLeft(RelationshipReference::MODIFIES, var, patternCl->syn.entity);
                std::vector<Value> resultUses = queryFacade->solveLeft(RelationshipReference::USES, expressionVar, patternCl->syn.entity);
            
                for (int i = 0; i < resultModifies.size(); i++) {
                    clauseResultModifies.insert(Tuple(std::vector{ resultModifies[i] }));
                }
                for (int i = 0; i < resultUses.size(); i++) {
                    clauseResultUses.insert(Tuple(std::vector{ resultUses[i] }));
                }

                return { clauseResultModifies, clauseResultUses };
            }
            else {
                std::vector<Value> result = queryFacade->getAssign(varName, patternCl->expression);
                ClauseResult clauseResult = ClauseResult(std::vector{ patternCl->syn });
                for (int i = 0; i < result.size(); i++) {
                    clauseResult.insert(Tuple(std::vector{ result[i] }));
                }
                return { clauseResult };
            }
        }
    }
}