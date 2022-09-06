#pragma once

#include "../Reference/Synonym.h"
#include <iostream>
#include <vector>

/*
 * Class encapsulating the list of synonyms declared.
 */
class Declaration
{
public:
	std::vector<Synonym> syns;
	Declaration();
    Declaration(std::vector<Synonym> syns);
};