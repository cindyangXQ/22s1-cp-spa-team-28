#pragma once

#include <map>
#include <unordered_set>

#include "../commons/Relationship.h"
#include "Table.h"

template <typename Left, typename Right>
class RelationshipsTable : public Table<Relationship<Left, Right>> {
public:
	/*
	* Stores a Relationship to both leftToRightsMap and rightToLeftsMap.
	*/
	void store(Relationship<Left, Right>* relationship) {
		Left left = relationship->getLeft();
		Right right = relationship->getRight();
		storeLeft(left, right);
		storeRight(right, left);
	}

	/*
	* Stores a Left to RightToLeftsMap.
	*/
	void storeRight(Right right, Left left) {
		auto key = this->rightToLeftsMap.find(right);

		if (key != this->rightToLeftsMap.end()) {
			key->second.insert(right);
		} else {
			rightToLeftsMap[right] = { left };
		}
	}

	/*
	* Stores a Right to LeftToRightsMap.
	*/
	void storeLeft(Left left, Right right) {
		auto key = this->leftToRightsMap.find(left);

		if (key != this->leftToRightsMap.end()) {
			key->second.insert(left);
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

	std::unordered_set<Left> retrieveRight(Right right) {
		return this->rightToLeftsMap[right];
	}

	int getTableSize() const {
		return -1; // TODO change behaviour, now returning dummy value
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

