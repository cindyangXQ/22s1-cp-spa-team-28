#pragma once

#include "TableValue.h"
#include "Reference.h"

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

enum class StatementHeader {
	LINE_NUMBER,
	STATEMENT_TYPE
};

/*
* Class encapsulating a SIMPLE Statement.
*/
class Statement : public TableValue, public Reference {
public:
	/*
	* Explicit constructor for a Statement.
	*/
	explicit Statement(int lineNo, StatementType type);
    Statement(int lineNo);

	StatementType getStatementType();

	int getLineNumber();

	/*
	* Override equality operator for Statement to check index and type.
	*/
	bool operator==(const Statement& other) const;

	/*
	* Checks whether value of StatementHeader is equal.
	*/
	bool isValueEqual(StatementHeader header, Statement* other);

	bool isStatementTypeEqual(Statement* other);

	bool isLineNumberEqual(Statement* other);

private:
	StatementType type;
	int lineNo;
};
