#include "../Reference/Synonym.h"
#include <iostream>
#include <vector>

class Declaration
{
public:
	std::vector<Synonym> syns;
	Declaration();
    Declaration(std::vector<Synonym> syns);
};