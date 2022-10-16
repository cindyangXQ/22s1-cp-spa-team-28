#include "ClauseResult.h"

void ClauseResult::insert(Tuple t) {
    this->isEmpty = false;
    this->table.insert(t);
}

ClauseTable ClauseResult::getTable() { return this->table; }
bool ClauseResult::getIsEmpty() { return this->isEmpty; }