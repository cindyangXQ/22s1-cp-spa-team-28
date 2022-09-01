#pragma once

#include "TableValue.h"

/*
* Enumerates the different kinds of SIMPLE statements.
*/
enum class StatementType {
	READ,
	PRINT,
	ASSIGN,
	CALL,
	WHILE,
	IF,
	NONE
};

/*
* Class encapsulating a SIMPLE Statement.
*/
class Statement : public TableValue {
public:
	/*
	* Explicit constructor for a Statement.
	*/
	explicit Statement(int index, StatementType type);

	StatementType getStatementType();

	int getIndex();

	/*
	* Override equality operator for Statement to check index and type.
	*/
	bool operator==(const Statement& other) const;

private:
	StatementType type;
	int index;
};
