#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "../commons/Relationship/Relationship.h"
#include "Table.h"
#include "StatementsTable.h"
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
		int left = std::stoi(leftRef.value.value);
		int right = std::stoi(rightRef.value.value);
		if (leftRef.isWildcard()) {
			return !rightToLeftsMap[right].empty();
		}
		if (rightRef.isWildcard()) {
			return !leftToRightsMap[left].empty();
		}
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
				if (rightToLeftsMap[left].size() != 0) {
					result.push_back(Value(ValueType::STMT_NUM, std::to_string(left)));
				}		
			}
		} else {
			int right = std::stoi(rightRef.value.value);
			for (int left : possibleLefts) {
				if (rightToLeftsMap[left].count(right) == 1) {
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
				if (rightToLeftsMap[left].count(right) == 1) {
					Value leftValue = Value(ValueType::STMT_NUM, std::to_string(left));
					Value rightValue = Value(ValueType::STMT_NUM, std::to_string(right));
					result.push_back(std::make_pair(leftValue, rightValue));
				}	
			}	
		}

		return result;
	}
};

class ParentTable : public StmtToStmtRelationshipsTable {

};

class ParentTTable : public StmtToStmtRelationshipsTable {

};

class FollowsTable : public StmtToStmtRelationshipsTable {

};

class FollowsTTable : public StmtToStmtRelationshipsTable {

};

class ModifiesSTable : public RelationshipsTable<int, std::string> {

};

class ModifiesPTable : public RelationshipsTable<std::string, std::string> {

};

class UsesSTable : public RelationshipsTable<int, std::string> {

};

class UsesPTable : public RelationshipsTable<std::string, std::string> {

};
