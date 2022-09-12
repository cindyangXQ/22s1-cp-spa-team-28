#pragma once
#include <vector>
#include "../../commons/Synonym.h"
#include "Tuple.h"

/*
* Encapsulate a table for storing synonyms and possible values the synonyms can be
*/
class ClauseTable {
public:
	std::vector<Synonym> header;
	std::vector<Tuple> rows;

	ClauseTable(std::vector<Synonym> header);
	/*
	* Given a list of synonyms, get the indices they correspond to
	*/
	std::vector<int> getIndices(std::vector<Synonym> common_headers);

	/*
	* Inserts a tuple into this table
	*/
	void insert(Tuple row);

	/*
	* Returns number of rows in this table
	*/
	size_t size();

	/*
	* Given two tables, find the common headers
	*/
	static std::vector<Synonym> getCommonHeaders(ClauseTable table1, ClauseTable table2);

	/*
	* Given two tables, construct a new table with all the unique headers from both tables
	*/
	static ClauseTable ConstructTable(ClauseTable table1, ClauseTable table2);

	static ClauseTable joinTables(ClauseTable table1, ClauseTable table2);
};
