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

/*
* Class encapsulating a SIMPLE Statement.
*/
class Statement {
public:
	explicit Statement(int index, StatementType type);

	StatementType getStatementType();

	int getIndex();

private:
	StatementType type;
	int index;
};