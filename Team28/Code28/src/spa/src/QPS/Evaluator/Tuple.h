#pragma once
#include <vector>
#include "../../commons/Value.h"
/*
* Encapsulates a single row in a table.
*/
class Tuple {
public:
	std::vector<Value> values;

	size_t size() {
		return values.size();
	}

	Tuple(std::vector<Value> values) {
		this->values = values;
	}

	/*
	* Return a pair of tuples, first tuple contains the values corresponding to the indices given in argument,
	* second tuple contains the remaining values.
	*/
	std::vector<Tuple> splitTuple(std::vector<int> indices) {
		int i, j;
		std::vector<Value> first;
		std::vector<Value> second;
		for (i = 0; i < indices.size(); i++) {
			first.push_back(this->values[indices[i]]);
		}
		bool inIndices;
		for (i = 0; i < this->values.size(); i++) {
			inIndices = false;
			for (j = 0; j < indices.size(); j++) {
				if (i == j) {
					inIndices = true;
				}
			}
			if (!inIndices) {
				second.push_back(this->values[indices[i]]);
			}
		}
		return std::vector<Tuple>{Tuple(first), Tuple(second)};
	}

	/*
	int hash();
	static combineSubTuple(Tuple tuple1, Tuple tuple2, Tuple tuple3); // tuple1 is the common sub-tuple
	*/
};