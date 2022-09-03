#include <string>

using namespace std;

class ModifyRel {
	string modifier;
	string modified;

public:
	ModifyRel(string modifier, string modified);
};

class ProcModifyRel : public ModifyRel {
public:
	ProcModifyRel(string procedure, string modified);
};

class StmtModifyRel : public ModifyRel {
public:
	StmtModifyRel(string statement, string modified);
};

class RdStModifyRel : public ModifyRel {
public:
	RdStModifyRel(string statement, string modified);
};

class AsgStModifyRel : public ModifyRel {
public:
	AsgStModifyRel(string statement, string modified);
};
