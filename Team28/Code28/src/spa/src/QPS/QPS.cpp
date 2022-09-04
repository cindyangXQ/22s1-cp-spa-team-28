#include "QPS.h"

std::string QPS::processQuery(std::string input) {
	SolvableQuery solvableQ = QueryParser::parse(input);
	QueryResult queryResult = evaluator.evaluate(&solvableQ);
	std::vector<std::string> result = evaluator.interpretQueryResult(&queryResult);
	if (result.size() == 0) {
		std::cout << "None" << std::endl;
		return "None";
	}
	std::string print;
	for (int i = 0; i < result.size() - 1; i++) {
		std::cout << result[i] + ", ";
		print.append(result[i] + ", ");
	}
	std::cout << result[result.size() - 1] << std::endl;
	print.append(result[result.size() - 1]);
	return print;
}