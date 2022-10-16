#pragma once
#include "../../commons/Value.h"
#include <vector>

// TOFIX: separate methods into .cpp file

/*
 * Encapsulates a single row in a table.
 */
class Tuple {
public:
    std::vector<Value> values;
    Tuple(std::vector<Value> values) { this->values = values; }

    size_t size();
    /*
     * Return a pair of tuples, first tuple contains the values corresponding to
     * the indices given in argument, second tuple contains the remaining
     * values.
     */
    std::vector<Tuple> splitTuple(std::vector<int> indices);

    static Tuple combineSubTuples(std::vector<Tuple> subTuples);

    bool equal(Tuple rhs);

    Value getValue(int index);
};
