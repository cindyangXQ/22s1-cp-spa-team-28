#pragma once
#include "ClauseTable.h"

/*
 * Encapsulate a table for storing result of a clause.
*/
class ClauseResult {
public:
    ClauseTable table;
    bool isEmpty = true;

    /*
     * Initialize an empty ClauseResult.
    */
    ClauseResult(bool isEmpty) : isEmpty(isEmpty){};

    /*
     * Explicit constructor for a ClauseResult.
    */
    explicit ClauseResult(std::vector<Reference> headers)
        : table(ClauseTable(headers)){};

public:
    /*
     * Inserts a tuple into the table.
    */
    void insert(Tuple t) {
        this->isEmpty = false;
        this->table.insert(t);
    }
};
