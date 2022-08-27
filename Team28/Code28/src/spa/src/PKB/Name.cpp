#include "Name.h"

Name::Name(int index, std::string nameString) {
	this->index = index;
	this->nameString = nameString;
}

std::string Name::getNameString() {
	return this->nameString;
}

int Name::getIndex() {
	return this->index;
}
