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
	* Stores the given Statement in a StatementsTable.
	*/
	void storeStatement(Statement *statement);

	/*
	* Stores the given Variable in a VariablesTable.
	*/
	void storeVariable(Variable *variable);

	/*
	* Stores the given Constant in a ConstantsTable.
	*/
	void storeConstant(Constant *constant);

	/*
	* Stores the given Procedure in a ProceduresTable.
	*/
	void storeProcedure(Procedure *procedure);

private:
	Storage *storage;
};
