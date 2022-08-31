#pragma once
#include "StatementReference.h"
class StatementNumber : public StatementReference {
public:
	int stmtNum = 0;
	StatementNumber(int num) {
		this->stmtNum = num;
	}
};