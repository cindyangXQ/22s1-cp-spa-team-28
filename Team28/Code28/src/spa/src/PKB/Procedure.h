#pragma once 

#include <string>

#include "Entity.h"
#include "NameDefinitions.h"

class Procedure : public Entity<ProcedureName> {
public:
	explicit Procedure(ProcedureName name);

	//TODO add representation of StmtLst in a Procedure
};
