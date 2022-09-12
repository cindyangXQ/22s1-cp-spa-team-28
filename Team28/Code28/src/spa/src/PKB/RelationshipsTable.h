#pragma once

#include <map>
#include <unordered_set>

#include "../commons/Relationship/Relationship.h"
#include "Table.h"
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

private:
	std::map<Left, std::unordered_set<Right>> leftToRightsMap;
	std::map<Right, std::unordered_set<Left>> rightToLeftsMap;
};

class ParentTable : public RelationshipsTable<int, int> {

};

class ParentTTable : public RelationshipsTable<int, int> {

};

class FollowsTable : public RelationshipsTable<int, int> {

};

class FollowsTTable : public RelationshipsTable<int, int> {

};

class ModifiesSTable : public RelationshipsTable<int, std::string> {

};

class ModifiesPTable : public RelationshipsTable<std::string, std::string> {

};

class UsesSTable : public RelationshipsTable<int, std::string> {

};

class UsesPTable : public RelationshipsTable<std::string, std::string> {

};
