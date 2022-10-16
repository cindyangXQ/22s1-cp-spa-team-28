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

    size_t size() { return values.size(); }

    Tuple(std::vector<Value> values) { this->values = values; }

    /*
     * Return a pair of tuples, first tuple contains the values corresponding to
     * the indices given in argument, second tuple contains the remaining
     * values.
     */
    std::vector<Tuple> splitTuple(std::vector<int> indices) {
        std::vector<Value> first;
        std::vector<Value> second;
        for (unsigned int i = 0; i < indices.size(); i++) {
            size_t index = (size_t)indices[i];
            Value v = values[index];
            first.push_back(v);
        }
        bool inIndices;
        // TOFIX: unsigned int can be int instead?
        for (unsigned int i = 0; i < this->values.size(); i++) {
            inIndices = false;
            for (unsigned int j = 0; j < indices.size(); j++) {
                if ((int)i == indices[j]) {
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
        for (unsigned int i = 0; i < subTuples.size(); i++) {
            for (unsigned int j = 0; j < subTuples[i].values.size(); j++) {
                new_values.push_back(subTuples[i].values[j]);
            }
        }
        return Tuple(new_values);
    }

    // first if can be a guard clause
    bool equal(Tuple rhs) {
        if (this->size() != rhs.size()) {
            return false;
        } else {
            for (unsigned int i = 0; i < this->size(); i++) {
                if (this->values[i].value != rhs.values[i].value) {
                    return false;
                }
            }
            return true;
        }
    }

    Value getValue(int index) { return this->values[(size_t)index]; }
};
