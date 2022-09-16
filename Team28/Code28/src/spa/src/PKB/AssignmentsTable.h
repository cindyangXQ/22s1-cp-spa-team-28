#pragma once

#include <map>
#include <string>
#include <unordered_set>

#include "../commons/Assignment.h"
#include "StatementPredicateMap.h"
#include "Table.h"

typedef std::pair<int, std::string> IntStringPair;

// To allow for computation of hash of pairs
// Referenced from https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
namespace std {
template <> struct hash<IntStringPair> {
    inline std::size_t operator()(const IntStringPair &v) const {
		std::size_t h1 = std::hash<int>{}(v.first);
		std::size_t h2 = std::hash<std::string>{}(v.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1<<6) + (h1>>2));
    }
};
}

/*
* Class encapsulating a Table used to store SIMPLE statements.
*/
class AssignmentsTable : public Table<Assignment> {
public:
	AssignmentsTable();

	/*
	* Stores an Assignment into the AssignmentsTable.
	* Mapping will be done from variable -> (lineNo, expression)
	* as well as expression -> (lineNo, variable)
	*/
	void store(Assignment* assignment);

	/*
	* Stores the mapping of variable to (lineNo, expression)
	*/
	void storeVariableMap(std::string variable, int lineNo, std::string expression);

	/*
	* Stores the mapping of expression to (lineNo, variable)
	*/
	void storeExpressionMap(std::string expression, int lineNo, std::string variable);

	/*
	* Retrieves the mapping of variable to (lineNo, expression)
	*/
	std::unordered_set<IntStringPair> retrieveFromVariable(std::string variable);

	/*
	* Retrieves the mapping of expression to (lineNo, variable)
	*/
	std::unordered_set<IntStringPair> retrieveFromExpression(std::string expression);

	int getTableSize() const {
		return -1; // TODO change behaviour, now returning dummy value
	}

private:
	std::map<std::string, std::unordered_set<IntStringPair>> variableMap;
	std::map<std::string, std::unordered_set<IntStringPair>> expressionMap;
};
