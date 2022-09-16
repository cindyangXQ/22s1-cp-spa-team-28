#include "AssignmentsTable.h"

AssignmentsTable::AssignmentsTable() = default;

void AssignmentsTable::store(Assignment* assignment) {
	std::string variable = assignment->getVariable();
	std::string expression = assignment->getExpression();
	int lineNo = assignment->getLineNo();
	storeVariableMap(variable, lineNo, expression);
	storeExpressionMap(expression, lineNo, variable);
};

void AssignmentsTable::storeVariableMap(std::string variable, int lineNo, std::string expression) {
	auto key = this->variableMap.find(variable);

	if (key != this->variableMap.end()) {
		key->second.insert(IntStringPair(lineNo, expression));
	} else {
		this->variableMap[variable] = {IntStringPair(lineNo, expression)};
	}
};

void AssignmentsTable::storeExpressionMap(std::string expression, int lineNo, std::string variable) {
	auto key = this->expressionMap.find(expression);

	if (key != this->expressionMap.end()) {
		key->second.insert(IntStringPair(lineNo, variable));
	} else {
		this->expressionMap[expression] = {IntStringPair(lineNo, variable)};
	}
};

std::unordered_set<IntStringPair> AssignmentsTable::retrieveFromVariable(std::string variable) {
	return this->variableMap[variable];
};

std::unordered_set<IntStringPair> AssignmentsTable::retrieveFromExpression(std::string expression) {
	return this->expressionMap[expression];
};
