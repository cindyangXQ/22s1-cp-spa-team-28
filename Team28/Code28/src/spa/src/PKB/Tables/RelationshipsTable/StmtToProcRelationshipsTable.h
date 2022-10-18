#pragma once

#include "StmtToNameRelationshipsTable.h"

class StmtToProcRelationshipsTable : public StmtToNameRelationshipsTable {};

class CallProcTable : public StmtToProcRelationshipsTable {};
