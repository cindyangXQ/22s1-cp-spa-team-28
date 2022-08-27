#include <iostream>
#include "../Reference/Synonym.h"
class SelectClause
{
public:
	Synonym syn;
    SelectClause();
    SelectClause(string s);
	SelectClause(Synonym syn);
};