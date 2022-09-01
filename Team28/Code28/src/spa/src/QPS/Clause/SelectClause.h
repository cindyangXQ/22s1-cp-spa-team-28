#pragma once

#include <iostream>
#include "../Reference/Synonym.h"
class SelectClause
{
public:
	Synonym syn;
    SelectClause();
	SelectClause(Synonym syn);
};