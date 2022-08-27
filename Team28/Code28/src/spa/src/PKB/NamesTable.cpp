#include "NamesTable.h"

void NamesTable::store(Name* name) {
	int index = name->getIndex();
	std::string nameString = name->getNameString();

	this->indexes.insert(index);
	this->indexNameMap[index] = name;
	this->tableSize++;
}

Name* NamesTable::retrieve(const int& index) {
	//currently assumed that table index starts from 1
	if (index <= 0 || index > this->tableSize) {
		return nullptr;
	}

	return this->indexNameMap.at(index - 1);
}

int NamesTable::getTableSize() const {
	return this->tableSize;
}
