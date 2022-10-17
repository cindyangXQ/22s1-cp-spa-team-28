#pragma once

#include "../../commons/TableValue.h"

/*
 * Abstract class to encapsulate expected behaviours for tables in PKB.
 */
class Table {
public:
    virtual void store(TableValue *item) = 0;

    virtual std::vector<std::string> getAllAsString() = 0;
};
