#pragma once

#include <map>
#include <unordered_set>

#include "Name.h"
#include "Table.h"

/*
* A table used to store Names.
*/
class NamesTable : public Table<Name> {
public:
	/*
	* Stores a Name into the NameTable.
	*/
	void store(Name* name);

	/*
	* Retrieves a Name from StatementsTable by index.
	*/
	Name* retrieve(const int& index);

	/*
	* Returns the size of NameTable.
	*/
	int getTableSize() const;

private:
	int tableSize = 0;
	std::unordered_set<int> indexes;
	std::map<int, Name*> indexNameMap;
};
