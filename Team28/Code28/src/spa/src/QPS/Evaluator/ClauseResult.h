#pragma once
#include "ClauseTable.h"

/*
 * Encapsulate a table for storing result of a clause.
 */
class ClauseResult {
public:
    /*
     * Initialize an empty ClauseResult.
     */
    ClauseResult(bool isEmpty) : isEmpty(isEmpty){};

    /*
     * Explicit constructor for a ClauseResult.
     */
    explicit ClauseResult(std::vector<Reference> headers)
        : table(ClauseTable(headers)){};
    /*
     * Inserts a tuple into the table.
     */
    void insert(Tuple t);

    ClauseTable getTable();
    bool getIsEmpty();

private:
    ClauseTable table;
    bool isEmpty = true;
};
