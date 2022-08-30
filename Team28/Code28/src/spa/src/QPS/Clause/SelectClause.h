#include <iostream>
#include "../Reference/Synonym.h"
class SelectClause
{
public:
	Synonym syn;
    SelectClause();
    SelectClause(std::string desc);
	SelectClause(Synonym syn);
};