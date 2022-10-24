#include "Procedure.h"

Procedure::Procedure(std::string_view name, int startStmt) : Entity(name.data()) {
    this->startStmtNum = startStmt;
};

int Procedure::getStmtNum() { return this->startStmtNum; };
