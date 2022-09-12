#include "PopulateFacade.h"

PopulateFacade::PopulateFacade(Storage *storage) {
	this->storage = storage;
};

void PopulateFacade::storeStatements(std::vector<Statement*> *statements) {
	StatementsTable* statementsTable = (StatementsTable*) this->storage->getTable(TableName::STATEMENTS);
	
	for(Statement* statement : *statements) {
		statementsTable->store(statement);
	}	
};

void PopulateFacade::storeVariables(std::vector<Variable*> *variables) {
	VariablesTable* variableTable = (VariablesTable*) this->storage->getTable(TableName::VARIABLES);
	for(Variable* variable : *variables) {
		variableTable->store(variable);
	}
};

void PopulateFacade::storeConstants(std::vector<Constant*> *constants) {
	ConstantsTable* constantsTable = (ConstantsTable*) this->storage->getTable(TableName::CONSTANTS);
	for(Constant* constant : *constants) {
		constantsTable->store(constant);
	}
};

void PopulateFacade::storeProcedures(std::vector<Procedure*> *procedures) {
	ProceduresTable* proceduresTable = (ProceduresTable*) this->storage->getTable(TableName::PROCEDURES);
	for(Procedure* procedure : *procedures) {
		proceduresTable->store(procedure);
	}
};

void PopulateFacade::storeFollows(std::vector<Relationship<int, int>*>* follows) {
	FollowsTable* followsTable = (FollowsTable*)this->storage->getTable(TableName::FOLLOWS);
	for (Relationship<int, int>* follow : *follows) {
		followsTable->store(follow);
	}
}

void PopulateFacade::storeFollowsT(std::vector<Relationship<int, int>*>* followsT) {
	FollowsTTable* followsTTable = (FollowsTTable*)this->storage->getTable(TableName::FOLLOWS_T);
	for (Relationship<int, int>* follow : *followsT) {
		followsTTable->store(follow);
	}
}

void PopulateFacade::storeParent(std::vector<Relationship<int, int>*>* parents) {
	ParentTable* parentTable = (ParentTable*)this->storage->getTable(TableName::PARENT);
	for (Relationship<int, int>* parent : *parents) {
		parentTable->store(parent);
	}
}

void PopulateFacade::storeParentT(std::vector<Relationship<int, int>*>* parentsT) {
	ParentTTable* parentTTable = (ParentTTable*)this->storage->getTable(TableName::PARENT_T);
	for (Relationship<int, int>* parent : *parentsT) {
		parentTTable->store(parent);
	}
}

void PopulateFacade::storeModifiesS(std::vector<Relationship<int, std::string>*>* modifiesS) {
	ModifiesSTable* modifiesSTable = (ModifiesSTable*)this->storage->getTable(TableName::MODIFIES_S);
	for (Relationship<int, std::string>* modify : *modifiesS) {
		modifiesSTable->store(modify);
	}
}

void PopulateFacade::storeModifiesP(std::vector<Relationship<std::string, std::string>*>* modifiesP) {
	ModifiesPTable* modifiesPTable = (ModifiesPTable*)this->storage->getTable(TableName::MODIFIES_P);
	for (Relationship<std::string, std::string>* modify : *modifiesP) {
		modifiesPTable->store(modify);
	}
}

void PopulateFacade::storeUsesS(std::vector<Relationship<int, std::string>*>* usesS) {
	UsesSTable* usesSTable = (UsesSTable*)this->storage->getTable(TableName::USES_S);
	for (Relationship<int, std::string>* use : *usesS) {
		usesSTable->store(use);
	}
}

void PopulateFacade::storeUsesP(std::vector<Relationship<std::string, std::string>*>* usesP) {
	UsesPTable* modifiesPTable = (UsesPTable*)this->storage->getTable(TableName::USES_P);
	for (Relationship<std::string, std::string>* use : *usesP) {
		modifiesPTable->store(use);
	}
}
