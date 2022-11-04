#pragma once

#include <vector>

class UFDS {
private:
    std::vector<int> p, rank, setSizes;
    int numSets;

public:
    explicit UFDS(int N);
    int findSet(int i);
    bool isSameSet(int i, int j);
    void unionSet(int i, int j);
    int setSize(int i);
    int numDisjointSets();
};