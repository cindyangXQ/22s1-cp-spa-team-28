#pragma once

#include "../Solvable.h"
#include "RelationshipsTable.h"

class StmtToProcRelationshipsTable
    : public RelationshipsTable<int, std::string> {
public:
};

class CallProcTable : public StmtToProcRelationshipsTable {};
