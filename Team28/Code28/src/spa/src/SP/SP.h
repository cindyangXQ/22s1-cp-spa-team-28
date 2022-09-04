#include "../PKB/PopulateFacade.h"

using namespace std;

class SP {
	PopulateFacade* storage;

public:
	SP(PopulateFacade* facade);
	void parse(string filename);
};
