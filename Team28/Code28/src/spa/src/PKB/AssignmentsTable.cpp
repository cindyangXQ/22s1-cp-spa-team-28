#include "AssignmentsTable.h"

AssignmentsTable::AssignmentsTable() = default;

void AssignmentsTable::store(Assignment* assignment) {
	std::string variable = assignment->getVariable();
	std::string expression = assignment->getExpression();
	int lineNo = assignment->getLineNo();
	storeVariableMap(variable, lineNo, expression);
	storeExpressionMap(expression, lineNo, variable);
	this->allLineNumbers.push_back(Value(ValueType::STMT_NUM, std::to_string(lineNo)));
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

std::vector<Value> AssignmentsTable::getAssignFromVarAndExpr(std::string varName, std::string expression) {
	if (expression == "_") {
		expression = "";
	} else {
		expression = "(" + expression + ")";
	}
	std::cout << "[DEBUG] converted expression" << std::endl;

	std::vector<Value> result;
	std::unordered_set<IntStringPair> map = this->retrieveFromVariable(varName);
	std::cout << "[DEBUG] retrieved VariableMap" << std::endl;

	for (auto itr = map.begin(); itr != map.end(); ++itr) {
		std::cout << "Expression: " << itr->second << std::endl;
		if (itr->second.find(expression) != std::string::npos) {
			std::cout << "Add LineNo: " << std::to_string(itr->first) << std::endl;
			result.push_back(Value(ValueType::STMT_NUM, std::to_string(itr->first)));
		}
	}
};

std::vector<Value> AssignmentsTable::getAssignFromExpr(std::string expression) {
	expression = "(" + expression + ")";
	std::vector<Value> result;
	std::cout << "[DEBUG] in getAssignFromExpr" << std::endl;
	std::map<std::string, std::unordered_set<IntStringPair>> expressionMap = this->expressionMap;

	for (auto const&[key, val] : expressionMap) {
		std::cout << "[DEBUG] key is " << key << std::endl;
		if (key.find(expression) == std::string::npos) {
			continue;
		}
		std::cout << "[DEBUG] add to result" << std::endl;
		for (IntStringPair pair : val) {
			result.push_back(Value(ValueType::STMT_NUM, std::to_string(pair.first)));
		}
	}
};

std::vector<Value> AssignmentsTable::getAssign(std::string varName, std::string expression) {
	if (varName == "_" && expression == "_") {
		return this->allLineNumbers;
	}
	if (varName == "_") {
		// Assume only partial match
		std::cout << "[DEBUG] wildcard Varname" << std::endl;
		return this->getAssignFromExpr(expression);
	}
	return this->getAssignFromVarAndExpr(varName, expression);
};

std::vector<std::pair<Value, Value>> AssignmentsTable::getAssignAndVar(std::string expression) {
	return {};
};
