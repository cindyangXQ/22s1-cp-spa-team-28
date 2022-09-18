#include "../PKB/PopulateFacade.h"

class SP {
    PopulateFacade *storage;

public:
    SP(PopulateFacade *facade);
    void parse(std::string filename);
};
