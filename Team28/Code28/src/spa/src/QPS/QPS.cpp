#include "QPS.h"

std::string QPS::processQuery(std::string input) {
    SolvableQuery solvableQ = QueryParser::parse(input);
    QueryResult queryResult = evaluator.evaluate(&solvableQ);
    std::vector<std::string> result =
        evaluator.interpretQueryResult(&queryResult);
    if (result.size() == 0) {
        return "";
    }
    std::string print;
    for (int i = 0; i < result.size() - 1; i++) {
        print.append(result[i] + ", ");
    }
    print.append(result[result.size() - 1]);
    return print;
}

void QPS::evaluate(std::string query, std::list<std::string> &results) {
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
    return;
}
