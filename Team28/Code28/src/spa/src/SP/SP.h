#pragma once

#include "../PKB/Facades/PopulateFacade.h"

class SP {
    PopulateFacade *storage;

public:
    SP(PopulateFacade *facade);
    void parse(std::string filename);
};
