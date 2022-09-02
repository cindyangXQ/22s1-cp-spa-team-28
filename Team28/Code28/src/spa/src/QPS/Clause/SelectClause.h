#pragma once

#include <iostream>
#include "../Reference/Synonym.h"

/*
 * Class encapsulating the logic of the select clause.
 */
class SelectClause
{
public:
	Synonym syn;
    SelectClause();
	SelectClause(Synonym syn);
};