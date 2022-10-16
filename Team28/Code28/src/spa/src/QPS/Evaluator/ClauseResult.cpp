#include "ClauseResult.h"

void ClauseResult::insert(Tuple t) {
    this->isEmpty = false;
    this->table.insert(t);
}