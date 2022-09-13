#pragma once
#include "ClauseTable.h"
class ClauseResult {
public:
    ClauseTable table;
    bool isEmpty = true;
    ClauseResult();
    ClauseResult(bool empty) {
        this->isEmpty = empty;
    }
    explicit ClauseResult(std::vector<Synonym> headers) : table(ClauseTable(headers)) {};

    void insert(Tuple t) {
        this->isEmpty = false;
        this->table.insert(t);
    }
};