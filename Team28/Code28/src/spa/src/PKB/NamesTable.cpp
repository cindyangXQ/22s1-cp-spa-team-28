#include "NameTable.h"
#include "NamesTable.h"

void NamesTable::store(Name* name) {
	int index = name->getIndex();
	std::string nameString = name->getNameString();

	this->indexes.insert(index);
	this->indexNameMap[index] = name;
	this->tableSize++;
}

Name* NamesTable::retrieve(const int& index) {
	Name* retrievedName = this->indexNameMap.find(index);

	if (retrievedName == this->indexNameMap.end()) {
		return nullptr;
	}

	return retrievedName;
}

int NamesTable::getTableSize() const {
	return this->tableSize;
}
