#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "../commons/Relationship/Relationship.h"
#include "Table.h"
#include "StatementsTable.h"
#include "NamesTable.h"
#include "RelationshipPredicateMap.h"

template <typename Left, typename Right>
class RelationshipsTable : public Table<Relationship<Left, Right>> {
public:
	/*
	* Stores a Relationship to both leftToRightsMap and rightToLeftsMap.
	*/
	void store(Relationship<Left, Right>* relationship) {
		Left left = relationship->getLeft();
		Right right = relationship->getRight();
		storeRightToLeftMap(left, right);
		storeLeftToRightMap(right, left);
	}

	/*
	* Stores a Left to RightToLeftsMap.
	*/
	void storeLeftToRightMap(Right right, Left left) {
		auto key = this->rightToLeftsMap.find(right);

		if (key != this->rightToLeftsMap.end()) {
			key->second.insert(left);
		} else {
			rightToLeftsMap[right] = { left };
		}
	}

	/*
	* Stores a Right to LeftToRightsMap.
	*/
	void storeRightToLeftMap(Left left, Right right) {
		auto key = this->leftToRightsMap.find(left);

		if (key != this->leftToRightsMap.end()) {
			key->second.insert(right);
		} else {
			leftToRightsMap[left] = { right };
		}
	}

	/*
	* Retrieves all Rights associated to a given Left.
	*/
	std::unordered_set<Right> retrieveLeft(Left left) {
		return this->leftToRightsMap[left];
	}

	/*
	* Retrieves all Lefts associated to a given Right.
	*/
	std::unordered_set<Left> retrieveRight(Right right) {
		return this->rightToLeftsMap[right];
	}

	/*
	* Filters RelationshipsTable based on conditions encapsulated in a given predicateMap.
	*/
	RelationshipsTable<Left, Right>* filter(RelationshipPredicateMap<Left, Right>* predicateMap) {
		if ((*predicateMap).isEmpty()) {
			RelationshipsTable<Left, Right>* newTable = this;
			return newTable;
		}

		RelationshipsTable<Left, Right>* newTable = new RelationshipsTable<Left, Right>();
		std::map<RelationshipHeader, Relationship<Left, Right>*> extractedMap = (*predicateMap).getPredicateMap();

		for (auto const& [mapToCheck, relationship] : extractedMap) {
			Left leftValue = relationship->getLeft();
			Right rightValue = relationship->getRight();

			if (mapToCheck == RelationshipHeader::CHECK_LEFT) {
				std::unordered_set<Right> set = this->leftToRightsMap[leftValue];
				if (set.find(rightValue) != set.end()) {
					newTable->storeLeftToRightMap(rightValue, leftValue);
				}
			}

			if (mapToCheck == RelationshipHeader::CHECK_RIGHT) {
				std::unordered_set<Left> set = this->rightToLeftsMap[rightValue];
				if (set.find(leftValue) != set.end()) {
					newTable->storeRightToLeftMap(leftValue, rightValue);
				}
			}
		}

		return newTable;
	}

	int getTableSize() const {
		return -1; // TODO change behaviour, now returning dummy value
	}

	std::map<Left, std::unordered_set<Right>> getLeftMap() {
		return this->leftToRightsMap;
	}

	std::map<Right, std::unordered_set<Left>> getRightMap() {
		return this->rightToLeftsMap;
	}

protected:
	std::map<Left, std::unordered_set<Right>> leftToRightsMap;
	std::map<Right, std::unordered_set<Left>> rightToLeftsMap;
};

class StmtToVarRelationshipsTable : public RelationshipsTable<int, std::string> {
public:
	/*
	* Returns true if the relationship holds between leftReference and rightReference.
	*/
	bool validate(Reference leftRef, Reference rightRef) {
		if (leftRef.isWildcard() && rightRef.isWildcard()) {
			return !leftToRightsMap.empty();
		}
		if (leftRef.isWildcard()) {
			std::string right = rightRef.value.value;
			return !rightToLeftsMap[right].empty();
		}
		if (rightRef.isWildcard()) {
			int left = std::stoi(leftRef.value.value);
			return !leftToRightsMap[left].empty();
		}
		int left = std::stoi(leftRef.value.value);
		std::string right = rightRef.value.value;
		return leftToRightsMap[left].count(right) == 1;
	}

	/*
	* Returns list of possible values that the right synonym can be.
	*/
	std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym, VariablesTable* variables) {
		// Validate rightSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(rightSynonym) == 0) {
			return std::vector<Value>();
		}
		// TODO: iterate through set don't convert to vector
		std::unordered_set<std::string> possibleRightsSet = variables->getAll();
		std::vector<std::string> possibleRights = std::vector<std::string>(possibleRightsSet.begin(), possibleRightsSet.end());
		std::vector<Value> result;
		if (leftRef.isWildcard()) {
			for (std::string right : possibleRights) {
				if (rightToLeftsMap[right].size() != 0) {
					result.push_back(Value(ValueType::VAR_NAME, right));
				}
			}
		}
		else {
			int left = std::stoi(leftRef.value.value);
			for (std::string right : possibleRights) {
				if (rightToLeftsMap[right].count(left) == 1) {
					result.push_back(Value(ValueType::VAR_NAME, right));
				}
			}
		}
		return result;
	};

	/*
	* Returns list of possible values that the left synonym can be.
	*/
	std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym, StatementsTable* statements) {
		// Validate leftSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(leftSynonym) == 0) {
			return std::vector<Value>();
		}
		std::vector<int> possibleLefts;
		if (leftSynonym == EntityName::STMT) {
			possibleLefts = statements->getAllLineNumbers();
		}
		else {
			StatementType statementType = Statement::getStmtTypeFromEntityName(leftSynonym);
			possibleLefts = statements->getStatementsByType(statementType);
		}
		std::vector<Value> result;
		if (rightRef.isWildcard()) {
			for (int left : possibleLefts) {
				if (leftToRightsMap[left].size() != 0) {
					result.push_back(Value(ValueType::STMT_NUM, std::to_string(left)));
				}
			}
		}
		else {
			std::string right = rightRef.value.value;
			for (int left : possibleLefts) {
				if (leftToRightsMap[left].count(right) == 1) {
					result.push_back(Value(ValueType::STMT_NUM, std::to_string(left)));
				}
			}
		}
		return result;
	};

	/*
	* Returns list of possible (Value, Value) that the pair of synonyms can be.
	*/
	std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym, EntityName rightSynonym, StatementsTable* statements, VariablesTable* variables) {
		// Validate leftSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(leftSynonym) == 0 || stmtRefSet.count(rightSynonym) == 0) {
			return std::vector<std::pair<Value, Value>>();
		}
		std::vector<int> possibleLefts;
		// TODO: iterate through set don't convert to vector
		std::unordered_set<std::string> possibleRightsSet = variables->getAll();
		std::vector<std::string> possibleRights = std::vector<std::string>(possibleRightsSet.begin(), possibleRightsSet.end());
		if (leftSynonym == EntityName::STMT) {
			possibleLefts = statements->getAllLineNumbers();
		}
		else {
			StatementType statementType = Statement::getStmtTypeFromEntityName(leftSynonym);
			possibleLefts = statements->getStatementsByType(statementType);
		}

		std::vector<std::pair<Value, Value>> result;
		for (int left : possibleLefts) {
			for (std::string right : possibleRights) {
				if (leftToRightsMap[left].count(right) == 1) {
					Value leftValue = Value(ValueType::STMT_NUM, std::to_string(left));
					Value rightValue = Value(ValueType::VAR_NAME, right);
					result.push_back(std::make_pair(leftValue, rightValue));
				}
			}
		}

		return result;
	}
};

class ProcToVarRelationshipsTable : public RelationshipsTable<std::string, std::string> {
public:
	/*
	* Returns true if the relationship holds between leftReference and rightReference.
	*/
	bool validate(Reference leftRef, Reference rightRef) {
		// TODO: Better way to handle wildcards
		if (leftRef.isWildcard() && rightRef.isWildcard()) {
			return !leftToRightsMap.empty();
		}

		if (leftRef.isWildcard()) {
			std::string right = rightRef.value.value;
			return !rightToLeftsMap[right].empty();
		}
		if (rightRef.isWildcard()) {
			std::string left =leftRef.value.value;
			return !leftToRightsMap[left].empty();
		}
		std::string left = leftRef.value.value;
		std::string right = rightRef.value.value;
		return leftToRightsMap[left].count(right) == 1;
	};

	/*
	* Returns list of possible values that the right synonym can be.
	*/
	std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym, VariablesTable* variables) {
		// Validate rightSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(rightSynonym) == 0) {
			return std::vector<Value>();
		}
		// TODO: iterate through set don't convert to vector
		std::unordered_set<std::string> possibleRightsSet = variables->getAll();
		std::vector<std::string> possibleRights = std::vector<std::string>(possibleRightsSet.begin(), possibleRightsSet.end());
		std::vector<Value> result;
		if (leftRef.isWildcard()) {
			for (std::string right : possibleRights) {
				if (rightToLeftsMap[right].size() != 0) {
					result.push_back(Value(ValueType::VAR_NAME, right));
				}
			}
		}
		else {
			std::string left = leftRef.value.value;
			for (std::string right : possibleRights) {
				if (rightToLeftsMap[right].count(left) == 1) {
					result.push_back(Value(ValueType::VAR_NAME, right));
				}
			}
		}
		return result;
	};

	/*
	* Returns list of possible values that the left synonym can be.
	*/
	std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym, ProceduresTable* procedures) {
		// Validate leftSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(leftSynonym) == 0) {
			return std::vector<Value>();
		}
		// TODO: iterate through set don't convert to vector
		std::unordered_set<std::string> possibleLeftsSet = procedures->getAll();
		std::vector<std::string> possibleLefts = std::vector<std::string>(possibleLeftsSet.begin(), possibleLeftsSet.end());
		std::vector<Value> result;
		if (rightRef.isWildcard()) {
			for (std::string left : possibleLefts) {
				if (leftToRightsMap[left].size() != 0) {
					// not sure if this is the same as procedure name
					result.push_back(Value(ValueType::VAR_NAME, left));
				}
			}
		}
		else {
			std::string right = rightRef.value.value;
			for (std::string left : possibleLefts) {
				if (leftToRightsMap[left].count(right) == 1) {
					// not sure if this is the same as procedure name
					result.push_back(Value(ValueType::VAR_NAME, left));
				}
			}
		}
		return result;
	};

	/*
	* Returns list of possible (Value, Value) that the pair of synonyms can be.
	*/
	std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym, EntityName rightSynonym, ProceduresTable* procedures, VariablesTable* variables) {
		// Validate leftSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(leftSynonym) == 0 || stmtRefSet.count(rightSynonym) == 0) {
			return std::vector<std::pair<Value, Value>>();
		}
		// TODO: iterate through set don't convert to vector
		std::unordered_set<std::string> possibleLeftsSet = procedures->getAll();
		std::vector<std::string> possibleLefts = std::vector<std::string>(possibleLeftsSet.begin(), possibleLeftsSet.end());
		std::unordered_set<std::string> possibleRightsSet = variables->getAll();
		std::vector<std::string> possibleRights = std::vector<std::string>(possibleRightsSet.begin(), possibleRightsSet.end());

		std::vector<std::pair<Value, Value>> result;
		for (std::string left : possibleLefts) {
			for (std::string right : possibleRights) {
				if (leftToRightsMap[left].count(right) == 1) {
					// not sure if this is the same as procedure name
					Value leftValue = Value(ValueType::VAR_NAME, left);
					Value rightValue = Value(ValueType::VAR_NAME, right);
					result.push_back(std::make_pair(leftValue, rightValue));
				}
			}
		}

		return result;
	}
};

class StmtToStmtRelationshipsTable : public RelationshipsTable<int, int> {
public:
	/*
	* Returns true if the relationship holds between leftReference and rightReference.
	*/
	bool validate(Reference leftRef, Reference rightRef) {
		// TODO: Better way to handle wildcards
		if (leftRef.isWildcard() && rightRef.isWildcard()) {
			return !leftToRightsMap.empty();
		}

		if (leftRef.isWildcard()) {
			int right = std::stoi(rightRef.value.value);
			return !rightToLeftsMap[right].empty();
		}
		if (rightRef.isWildcard()) {
			int left = std::stoi(leftRef.value.value);
			return !leftToRightsMap[left].empty();
		}		
		int left = std::stoi(leftRef.value.value);
		int right = std::stoi(rightRef.value.value);
		return leftToRightsMap[left].count(right) == 1;
	};

	/*
	* Returns list of possible values that the right synonym can be.
	*/
	std::vector<Value> solveRight(Reference leftRef, EntityName rightSynonym, StatementsTable* statements) {
		// Validate rightSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(rightSynonym) == 0) {
			return std::vector<Value>();
		}
		std::vector<int> possibleRights;
		if (rightSynonym == EntityName::STMT) {
			possibleRights = statements->getAllLineNumbers();
		} else {
			StatementType statementType = Statement::getStmtTypeFromEntityName(rightSynonym);
			possibleRights = statements->getStatementsByType(statementType);
		}
		std::vector<Value> result;
		if (leftRef.isWildcard()) {
			for (int right : possibleRights) {
				if (rightToLeftsMap[right].size() != 0) {
					result.push_back(Value(ValueType::STMT_NUM, std::to_string(right)));
				}		
			}
		} else {
			int left = std::stoi(leftRef.value.value);
			for (int right : possibleRights) {
				if (rightToLeftsMap[right].count(left) == 1) {
					result.push_back(Value(ValueType::STMT_NUM, std::to_string(right)));
				}		
			}
		}
		return result;
	};

	/*
	* Returns list of possible values that the left synonym can be.
	*/
	std::vector<Value> solveLeft(Reference rightRef, EntityName leftSynonym, StatementsTable* statements) {
		// Validate leftSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(leftSynonym) == 0) {
			return std::vector<Value>();
		}
		std::vector<int> possibleLefts;
		if (leftSynonym == EntityName::STMT) {
			possibleLefts = statements->getAllLineNumbers();
		} else {
			StatementType statementType = Statement::getStmtTypeFromEntityName(leftSynonym);
			possibleLefts = statements->getStatementsByType(statementType);
		}
		std::vector<Value> result;
		if (rightRef.isWildcard()) {
			for (int left : possibleLefts) {
				if (leftToRightsMap[left].size() != 0) {
					result.push_back(Value(ValueType::STMT_NUM, std::to_string(left)));
				}		
			}
		} else {
			int right = std::stoi(rightRef.value.value);
			for (int left : possibleLefts) {
				if (leftToRightsMap[left].count(right) == 1) {
					result.push_back(Value(ValueType::STMT_NUM, std::to_string(left)));
				}		
			}
		}
		return result;
	};
	
	/*
	* Returns list of possible (Value, Value) that the pair of synonyms can be.
	*/
	std::vector<std::pair<Value, Value>> solveBoth(EntityName leftSynonym, EntityName rightSynonym, StatementsTable* statements) {
		// Validate leftSynonym is a statement. TODO: throw error if not
		if (stmtRefSet.count(leftSynonym) == 0 || stmtRefSet.count(rightSynonym) == 0) {
			return std::vector<std::pair<Value, Value>>();
		}
		std::vector<int> possibleLefts;
		std::vector<int> possibleRights;
		if (leftSynonym == EntityName::STMT) {
			possibleLefts = statements->getAllLineNumbers();
		} else {
			StatementType statementType = Statement::getStmtTypeFromEntityName(leftSynonym);
			possibleLefts = statements->getStatementsByType(statementType);
		}
		if (rightSynonym == EntityName::STMT) {
			possibleRights = statements->getAllLineNumbers();
		} else {
			StatementType statementType = Statement::getStmtTypeFromEntityName(rightSynonym);
			possibleRights = statements->getStatementsByType(statementType);
		}

		std::vector<std::pair<Value,Value>> result;
		for (int left : possibleLefts) {
			for (int right : possibleRights) {
				if (leftToRightsMap[left].count(right) == 1) {
					Value leftValue = Value(ValueType::STMT_NUM, std::to_string(left));
					Value rightValue = Value(ValueType::STMT_NUM, std::to_string(right));
					result.push_back(std::make_pair(leftValue, rightValue));
				}	
			}	
		}

		return result;
	}
};

typedef StmtToStmtRelationshipsTable ParentTable;
typedef StmtToStmtRelationshipsTable ParentTTable;
typedef StmtToStmtRelationshipsTable FollowsTable;
typedef StmtToStmtRelationshipsTable FollowsTTable;
typedef StmtToVarRelationshipsTable ModifiesSTable;
typedef StmtToVarRelationshipsTable UsesSTable;
typedef ProcToVarRelationshipsTable ModifiesPTable;
typedef ProcToVarRelationshipsTable UsesPTable;
