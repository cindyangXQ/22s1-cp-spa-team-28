#pragma once

#include "Storage.h"

/*
* A Facade class for QPS->PKB interactions.
*/
class QueryFacade {
public:
	/*
	* Explicit constructor for a QueryFacade.
	*/
	explicit QueryFacade(Storage storage);

	/*
	* Returns all statements inside the StatementsTable.
	*/
	std::vector<Statement*> getAllStatements();

	/*
	* Returns all variable names inside the VariablesTable.
	*/
	std::vector<VariableName> getAllVariables();

	/*
	* Returns all constant names inside the ConstantsTable.
	*/
	std::vector<ConstantName> getAllConstants();

	/*
	* Returns all procedure names inside the ProceduresTable.
	*/
	std::vector<ProcedureName> getAllProcedures();

private:
	Storage storage;
};
