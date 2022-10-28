#pragma once

#include "../PKB/Facades/PopulateFacade.h"

/*
 * A Wrapper class of SP.
 */
class SP {
    PopulateFacade *storage;

public:
    SP(PopulateFacade *facade);

    /*
     * Parses the program in the given filename, and stores all extracted
     * Entities and Abstractions into PKB via PopulateFacade.
     */
    void parse(std::string filename);

    /*
     * Converts an expression into a fully bracket-ed format.
     */
    static std::string convertExpression(std::string input);
};
