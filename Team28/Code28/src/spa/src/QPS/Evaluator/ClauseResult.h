#pragma once
#include "ClauseTable.h"

// TOFIX: ClauseResult() function definition not found (warning)
// TOFIX: Class header comment / method comment

class ClauseResult {
public:
    ClauseTable table;
    bool isEmpty = true;
    ClauseResult();
    ClauseResult(bool isEmpty) : isEmpty(isEmpty){};
    explicit ClauseResult(std::vector<Reference> headers)
        : table(ClauseTable(headers)){};

    void insert(Tuple t) {
        this->isEmpty = false;
        this->table.insert(t);
    }
};
