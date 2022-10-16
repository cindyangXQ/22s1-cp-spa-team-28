#include "QPS.h"

void QPS::evaluate(std::string query, std::list<std::string> &results) {
    try {
        SolvableQuery solvableQ = QueryParser::parse(query);
        QueryResult queryResult = evaluator.evaluate(&solvableQ);
        std::vector<std::string> result =
            evaluator.interpretQueryResult(&queryResult);

        for (int i = 0; i < result.size(); i++) {
            results.push_back(result[i]);
        }
    } catch (std::runtime_error e) {
        results.push_back(e.what());
    }
}
