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
		Left& left = relationship->getLeft();
		Right& right = relationship->getRight();
		insertLeft(left, right);
		insertRight(right, left);
	}

	void insertRight(Left &left, Right &right) {
		auto key = this->rightToLeftsMap.find(right);

		if (key != this->rightToLeftsMap.end()) {
			key->second.insert(right);
		} else {
			rightToLeftsMap[right] = { left };
		}
	}

	void insertLeft(Left &left, Right &right) {
		auto key = this->leftToRightsMap.find(left);

		if (key != this->leftToRightsMap.end()) {
			key->second.insert(left);
		} else {
			leftToRightsMap[left] = { right };
		}
	}

	int getTableSize() const {
		return -1;
	}

private:
	std::map<Left, std::unordered_set<Right>> leftToRightsMap;
	std::map<Right, std::unordered_set<Left>> rightToLeftsMap;
};