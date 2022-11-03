#include "UFDS.h"

UFDS::UFDS(int N) {
    numSets = N;
    rank.assign((size_t)N, 0);
    p.assign((size_t)N, 0);
    for (int i = 0; i < N; i++) {
        p[(size_t)i] = i;
    }
    setSizes.assign((size_t)N, 1);
}
int UFDS::findSet(int i) {
    return (p[(size_t)i] == i) ? i : p[(size_t)i] = findSet(p[(size_t)i]);
}
bool UFDS::isSameSet(int i, int j) { return findSet(i) == findSet(j); }
void UFDS::unionSet(int i, int j) {
    if (!isSameSet(i, j)) {
        int x = findSet(i), y = findSet(j);
        if (rank[(size_t)x] > rank[(size_t)y]) {
            setSizes[(size_t)findSet(x)] += setSizes[(size_t)findSet(y)];
            p[(size_t)y] = x;
        } else {
            setSizes[(size_t)findSet(y)] += setSizes[(size_t)findSet(x)];
            p[(size_t)x] = y;
            if (rank[(size_t)x] == rank[(size_t)y])
                rank[(size_t)y]++;
        }
        numSets--;
    }
}
int UFDS::setSize(int i) { return setSizes[(size_t)findSet(i)]; }
int UFDS::numDisjointSets() { return numSets; }