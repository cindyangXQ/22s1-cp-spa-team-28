#include "PKB.h"

PKB::PKB() {
	this->storage = new Storage();
	this->populateFacade = new PopulateFacade(this->storage);
	this->queryFacade = new QueryFacade(this->storage);
};
