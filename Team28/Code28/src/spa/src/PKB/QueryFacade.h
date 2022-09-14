#pragma once

#include "Storage.h"
#include "StatementPredicateMap.h"

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

	/*
	* Returns all statements inside the StatementsTable.
	*/
	std::vector<Statement*> getAllStatementsByType(StatementType type);

	/*
	* Returns statement with the given line number.
	*/
	Statement* getStatementByLineNo(const int& lineNo);

	/*
	* Returns all variable names inside the VariablesTable.
	*/
	std::vector<std::string> getAllVariables();

	/*
	* Returns variable that matches a given name.
	*/
	Variable* getVariableByName(const std::string& name);

	/*
	* Returns all constant names inside the ConstantsTable.
	*/
	std::vector<std::string> getAllConstants();

	/*
	* Returns constant that matches a given name.
	*/
	Constant* getConstantByName(const std::string& name);

	/*
	* Returns all procedure names inside the ProceduresTable.
	*/
	std::vector<std::string> getAllProcedures();

	/*
	* Returns procedure that matches a given name.
	*/
	Procedure* getProcedureByName(const std::string& name);

	/*
	* Returns true if relationship holds between leftReference and rightReference.
	*/
	bool validate(RelationshipReference relType, Reference leftRef, Reference rightRef);

	/*
	* Returns list of possible values that the right synonym can be based on their relationship.
	*/
	std::vector<Value> solveRight(RelationshipReference relType, Reference leftRef, EntityName rightSynonym);

	/*
	* Returns list of possible values that the left synonym can be based on their relationship.
	*/
	std::vector<Value> solveLeft(RelationshipReference relType, Reference rightRef, EntityName leftSynonym);
	
	/*
	* Returns list of possible (Value, Value) that the pair of synonyms can be based on their relationship.
	*/
	std::vector<std::pair<Value, Value>> solveBoth(RelationshipReference relType, EntityName leftSynonym, EntityName rightSynonym);
	
private:
	Storage* storage;
};
