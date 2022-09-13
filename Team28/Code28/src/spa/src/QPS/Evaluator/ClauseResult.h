#pragma once
#include "ClauseTable.h"
class ClauseResult {
public:
    ClauseTable table;
    bool isEmpty;
    ClauseResult();
    ClauseResult(bool empty) {
        this->isEmpty = empty;
    }
    explicit ClauseResult(std::vector<Synonym> headers) : table(ClauseTable(headers)) {};
};