#pragma once
#include "Reference.h"

/*
 * Class encapsulating a statement number.
 */
class StatementNumber : public Reference {
public:
	int stmtNum = 0;
	StatementNumber(int num) {
		this->stmtNum = num;
	}
};