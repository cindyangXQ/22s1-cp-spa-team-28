#include "QPS.h"

std::string QPS::processQuery(std::string input) {
	SolvableQuery solvableQ = QueryParser::parse(input);
	QueryResult queryResult = evaluator.evaluate(&solvableQ);
	std::vector<std::string> result = evaluator.interpretQueryResult(&queryResult);
	if (result.size() == 0) {
		// std::cout << "None" << std::endl;
		return "None";
	}
	std::string print;
	for (int i = 0; i < result.size() - 1; i++) {
		// std::cout << result[i] + ", ";
		print.append(result[i] + ", ");
	}
	// std::cout << result[result.size() - 1] << std::endl;
	print.append(result[result.size() - 1]);
	return print;
}

void QPS::evaluate(std::string query, std::list<std::string>& results) {
	SolvableQuery solvableQ = QueryParser::parse(query);
	QueryResult queryResult = evaluator.evaluate(&solvableQ);
	std::vector<std::string> result = evaluator.interpretQueryResult(&queryResult);
	if (result.size() == 0) {
		// std::cout << "None" << std::endl;
		results.push_back("None");
		return;
	}
	std::string print;
	for (int i = 0; i < result.size(); i++) {
		// std::cout << result[i] + ", ";
		results.push_back(result[i]);
	}
	// std::cout << result[result.size() - 1] << std::endl;
	return;
}