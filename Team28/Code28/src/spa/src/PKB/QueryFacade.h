#include "Storage.h"

/*
* A Facade class for QPS->PKB interactions.
*/
class QueryFacade {
public:
	/*
	* Explicit constructor for a QueryFacade.
	*/
	explicit QueryFacade(Storage* storage);

	/*
	* Returns all statements inside the StatementsTable.
	*/
	std::vector<Statement*> getAllStatements();

	Statement* getStatementByLineNo(const int& lineNo);

	/*
	* Returns all variable names inside the VariablesTable.
	*/
	std::vector<VariableName> getAllVariables();

	/*
	* Returns variable that matches a given name.
	*/
	Variable* getVariableByName(const VariableName& name);

	/*
	* Returns all constant names inside the ConstantsTable.
	*/
	std::vector<ConstantName> getAllConstants();

	/*
	* Returns constant that matches a given name.
	*/
	Constant* getConstantByName(const ConstantName& name);

	/*
	* Returns all procedure names inside the ProceduresTable.
	*/
	std::vector<ProcedureName> getAllProcedures();

	/*
	* Returns procedure that matches a given name.
	*/
	Procedure* getProcedureByName(const ProcedureName& name);

private:
	Storage* storage;
};
