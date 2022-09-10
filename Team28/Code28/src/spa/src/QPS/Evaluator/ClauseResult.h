#pragma once
#include "Table.h"
class ClauseResult {
public:
    Table clauseTable;
    bool isTrue;
    ClauseResult();
    explicit ClauseResult(bool isTrue) : isTrue(isTrue) {};
};