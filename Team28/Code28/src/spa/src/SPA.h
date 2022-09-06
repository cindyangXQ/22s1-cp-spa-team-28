#pragma once

#include "PKB/PKB.h"
#include "QPS/QPS.h"
#include "SP/SP.h"

class SPA {
public:
	SPA();

	void parse(std::string filename);

	void evaluateQuery(std::string query, std::list<std::string>& results);

private:
	PKB pkb;
	QPS qps = QPS(nullptr);
	SP sp = SP(nullptr);
};
