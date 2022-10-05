#pragma once

#include "../../commons/Reference.h"
#include "Table.h"

class Solvable {
public:
    virtual bool validate(Reference leftRef, Reference rightRef) = 0;
};
