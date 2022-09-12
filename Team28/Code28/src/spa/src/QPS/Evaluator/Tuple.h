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
				second.push_back(this->values[i]);
			}
		}
		return std::vector<Tuple>{Tuple(first), Tuple(second)};
	}

	static Tuple combineSubTuples(std::vector<Tuple> subTuples) {
		std::vector<Value> new_values;
		int i, j;
		for (i = 0; i < subTuples.size(); i++) {
			for (j = 0; j < subTuples[i].values.size(); j++) {
				new_values.push_back(subTuples[i].values[j]);
			}
		}
		return Tuple(new_values);
	}

	bool operator == (Tuple t) {
		if (this->size() != t.size()) {
			return false;
		} else {
			for (int i = 0; i < this->size(); i++) {
				if (this->values[i].value != t.values[i].value) {
					return false;
				}
			}
			return true;
		} 
	}
};