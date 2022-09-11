#pragma once
#include "ClauseTable.h"
class ClauseResult {
public:
    bool isTrue;
    ClauseResult();
    explicit ClauseResult(bool isTrue) : isTrue(isTrue) {};
};