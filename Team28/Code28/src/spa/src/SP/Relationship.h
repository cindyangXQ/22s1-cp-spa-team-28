#include <string>

using namespace std;

class ModifyRel {
	string modifier;
	string modified;

public:
	ModifyRel(string modifier, string modified);
};

class ProcModifyRel : ModifyRel {
public:
	ProcModifyRel(string procedure, string modified);
};

class StmtModifyRel : ModifyRel {
public:
	StmtModifyRel(string statement, string modified);
};

class RdStModifyRel : StmtModifyRel {
public:
	RdStModifyRel(string statement, string modified);
};

class AsgStModifyRel : StmtModifyRel {
public:
	AsgStModifyRel(string statement, string modified);
};
