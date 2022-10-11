#include "Procedure.h"

Procedure::Procedure(std::string name, int startStmt) : Entity(name) {
    this->startStmtNum = startStmt;
};

int Procedure::getStmtNum() {
    return this->startStmtNum;
};
