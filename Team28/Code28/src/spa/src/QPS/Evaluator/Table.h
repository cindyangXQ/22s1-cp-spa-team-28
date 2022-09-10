#include "../Reference/Synonym.h"
#include <vector>

/*
* Encapsulate a table for storing synonyms and possible values the synonyms can be
*/
class Table {
	std::vector<Synonym> header;
	std::vector<Tuple> rows;

	std::vector<int> getTupleIndex(std::vector<Synonym> headers); // given a list of synonyms, get the tuple indices they correspond to

	static std::vector<Synonym> getCommonHeaders(Table1, Table2);
	static Table ConstructTable(std::vector<Synonym>);
	static Table ConstructTable(std::vector<Synonym>, std::vector<Synonym>); // result table header order: common headers, remaining headers from first argument, remaining headers from second argument
};