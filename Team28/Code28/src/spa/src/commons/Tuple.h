#pragma once
#include <vector>
#include "Value.h"
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
	std::vector<Tuple> splitTuple(std::vector<int>); // return a pair of tuples, first tuple contains the values corresponding to the indices given in argument, second tuple contains the remaining values.
	int hash();

	static combineSubTuple(Tuple tuple1, Tuple tuple2, Tuple tuple3); // tuple1 is the common sub-tuple
	*/
};