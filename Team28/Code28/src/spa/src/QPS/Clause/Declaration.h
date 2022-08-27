#include "../Reference/Synonym.h"
#include <iostream>
#include <vector>

class Declaration
{
public:
	vector<Synonym> syns;
	Declaration();
    Declaration(vector<Synonym> syns);
};