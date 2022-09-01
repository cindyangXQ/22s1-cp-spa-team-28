#pragma once
#include "Reference.h"
class StatementNumber : public Reference {
public:
	int stmtNum = 0;
	StatementNumber(int num) {
		this->stmtNum = num;
	}
};