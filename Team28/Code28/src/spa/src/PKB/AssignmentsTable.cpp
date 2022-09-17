#include "AssignmentsTable.h"

AssignmentsTable::AssignmentsTable() = default;

void AssignmentsTable::store(Assignment* assignment) {
	int lineNo = assignment->getLineNo();
	this->allAssignments.push_back(*assignment);
	this->allLineNumbers.push_back(Value(ValueType::STMT_NUM, std::to_string(lineNo)));

	this->tableSize += 1;
};

int AssignmentsTable::getTableSize() const {
	return this->tableSize;
};

std::vector<Value> AssignmentsTable::containsVarAndExpr(std::string varName, std::string expression) {
	std::vector<Value> result;
	
	for (Assignment assignment : this->allAssignments) {
		if (assignment.getVariable() == varName && assignment.getExpression() == expression) {
			result.push_back(Value(ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
		}
	}
	return result;
};


std::vector<Value> AssignmentsTable::getAssignFromVarAndExpr(std::string varName, std::string expression) {
	if (expression == "_") {
		expression = "";
	} else {
		expression = "(" + expression + ")";
	}

	std::vector<Value> result;

	for (Assignment assignment : this->allAssignments) {
		if (assignment.getVariable() != varName) {
			continue;
		}
		if (assignment.getExpression().find(expression) != std::string::npos) {
			result.push_back(Value(ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
		}
	}
	return result;
};

std::vector<Value> AssignmentsTable::getAssignFromExpr(std::string expression) {
	expression = "(" + expression + ")";
	std::vector<Value> result;

	for (Assignment assignment : this->allAssignments) {
		if (assignment.getExpression().find(expression) != std::string::npos) {
			result.push_back(Value(ValueType::STMT_NUM, std::to_string(assignment.getLineNo())));
		}
	}
	return result;
};

std::vector<Value> AssignmentsTable::getAssign(std::string varName, std::string expression) {
	if (varName == "_" && expression == "_") {
		return this->allLineNumbers;
	}
	if (varName == "_") {
		// Assume only partial match. TODO: handle exact matching
		return this->getAssignFromExpr(expression);
	}
	return this->getAssignFromVarAndExpr(varName, expression);
};

std::vector<std::pair<Value, Value>> AssignmentsTable::getAssignAndVar(std::string expression) {
	if (expression == "_") {
		expression = "";
	} else {
		expression = "(" + expression + ")";
	}
	// TODO: handle exact matching

	std::vector<std::pair<Value, Value>> result;
	for (Assignment assignment : this->allAssignments) {
		if (assignment.getExpression().find(expression) != std::string::npos) {
			result.push_back(std::make_pair(
				Value(ValueType::STMT_NUM, std::to_string(assignment.getLineNo())),
				Value(ValueType::VAR_NAME, assignment.getVariable())
			));
		}
	}
	return result;

};
