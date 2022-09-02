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

private:
	Storage storage;
};
