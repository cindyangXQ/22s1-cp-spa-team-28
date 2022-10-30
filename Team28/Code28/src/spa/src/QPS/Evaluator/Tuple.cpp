#include "Tuple.h"

size_t Tuple::size() { return values.size(); }

std::vector<Tuple> Tuple::splitTuple(std::vector<int> indices) {
    std::vector<Value> first;
    std::vector<Value> second;
    for (unsigned int i = 0; i < indices.size(); i++) {
        size_t index = (size_t)indices[i];
        Value v = values[index];
        first.push_back(v);
    }
    bool inIndices;

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

Tuple Tuple::combineSubTuples(std::vector<Tuple> subTuples) {
    std::vector<Value> new_values;
    for (unsigned int i = 0; i < subTuples.size(); i++) {
        for (unsigned int j = 0; j < subTuples[i].values.size(); j++) {
            new_values.push_back(subTuples[i].values[j]);
        }
    }
    return Tuple(new_values);
}

bool Tuple::equal(Tuple rhs) {
    if (this->size() != rhs.size()) {
        return false;
    }
    for (unsigned int i = 0; i < this->size(); i++) {
        if (this->values[i].getValue() != rhs.values[i].getValue()) {
            return false;
        }
    }
    return true;
}

Value Tuple::getValue(int index) { return this->values[(size_t)index]; }

std::vector<Value> Tuple::getValues() { return this->values; }

std::string Tuple::to_string() {
    std::string s = "";
    for (int i = 0; i < values.size(); i++) {
        s += " " + values[i].getValue();
    }
    return s;
}