#include "../Reference/Synonym.h"
#include <iostream>
#include <list>

class Declaration
{
public:
	list<Synonym> syns;
	Declaration();
    Declaration(list<Synonym> syns);
};