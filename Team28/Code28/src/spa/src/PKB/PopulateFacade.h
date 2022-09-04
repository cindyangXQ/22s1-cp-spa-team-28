#pragma once

#include <vector>

#include "Storage.h"

/*
* A Facade class for SP->PKB interactions.
*/
class PopulateFacade {
public:
	/*
	* Explicit constructor for a PopulateFacade.
	*/
	explicit PopulateFacade(Storage *storage);

	/*
	* Stores the given statements in a StatementsTable.
	*/
	void storeStatements(std::vector<Statement*> *statements);

	/*
	* Stores the given variables in a VariablesTable.
	*/
	void storeVariables(std::vector<Variable*> *variables);

	/*
	* Stores the given constants in a ConstantsTable.
	*/
	void storeConstants(std::vector<Constant*> *constants);

	/*
	* Stores the given procedures in a ProceduresTable.
	*/
	void storeProcedures(std::vector<Procedure*> *procedures);

private:
	Storage *storage;
};
