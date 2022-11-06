#include "QPS.h"

void QPS::evaluate(std::string query, std::list<std::string> &results) {
    try {
        SolvableQuery solvableQ = QueryParser::parse(query);
        optimizer.optimize(&solvableQ);
        QueryResult queryResult = evaluator.evaluate(&solvableQ);
        std::vector<std::string> result =
            evaluator.interpretQueryResult(&queryResult);

        for (int i = 0; i < result.size(); i++) {
            results.push_back(result[i]);
        }
    } catch (SyntaxError e) {
        results.push_back("SyntaxError");
    } catch (SemanticError e) {
        results.push_back("SemanticError");
    } catch (EmptyTableError e) {
        if (e.getIsBoolean()) {
            results.push_back("FALSE");
        } else {
            return;
        }
    }
}

QueryEvaluator QPS::getEvaluator() { return this->evaluator; }
