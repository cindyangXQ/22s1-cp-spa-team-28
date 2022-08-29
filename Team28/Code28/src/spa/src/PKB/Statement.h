#pragma once

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
	INDEX,
	STATEMENT_TYPE
};

/*
* Class encapsulating a SIMPLE Statement.
*/
class Statement {
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

	/*
	* Checks whether value of StatementHeader is equal
	*/
	bool isValueEqual(StatementHeader header, Statement* other);

	bool isStatementTypeEqual(Statement* other);

	bool isIndexEqual(Statement* other);

private:
	StatementType type;
	int index;
};