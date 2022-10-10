#include "QPS.h"

void QPS::evaluate(std::string query, std::list<std::string> &results) {
    try {
        SolvableQuery solvableQ = QueryParser::parse(query);
        QueryResult queryResult = evaluator.evaluate(&solvableQ);
        std::vector<std::string> result =
            evaluator.interpretQueryResult(&queryResult);
        if (result.size() == 0) {
            return;
        }
        std::string print;
        for (int i = 0; i < result.size(); i++) {
            results.push_back(result[i]);
        }
    } catch (SyntaxError e) {
        results.push_back("SyntaxError");
    } catch (SemanticError e) {
        results.push_back("SemanticError");
    }
}
