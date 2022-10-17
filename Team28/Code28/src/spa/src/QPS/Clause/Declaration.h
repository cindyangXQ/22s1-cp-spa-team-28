#pragma once

#include "../../commons/Synonym.h"
#include <iostream>
#include <vector>

/*
 * Class encapsulating the list of synonyms declared.
 */
class Declaration {
public:
    Declaration(){};
    Declaration(std::vector<Synonym> syns) : syns(syns){};

    /*
     * Return list of declared synonym.
     */
    std::vector<Synonym> getSyns();

private:
    std::vector<Synonym> syns;
};
