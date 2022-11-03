#include "PKB.h"

#include "Algorithms/ControlFlowGraph.h"

PKB::PKB() {
    this->storage = new Storage();
    this->populateFacade = new PopulateFacade(this->storage);
    this->queryFacade = new QueryFacade(this->storage);
};

void PKB::populateNext() {
    NextTable *nextTable = this->storage->getTable<NextTable>();
    StorageView *storageView = this->storage->getStorageView();
    ControlFlowGraph *cfg = new ControlFlowGraph(nextTable, storageView);
    cfg->populateNext();
    this->cfg = cfg;
}

// TODO: add a flag to only do work if populateNext has been executed
void PKB::initNextT() {
    NextTTable *nextTTable = this->storage->getTable<NextTTable>();
    nextTTable->initNextT(this->storage->getStorageView());
}

void PKB::initAffects() {
    AffectsTable *affectsTable = this->storage->getTable<AffectsTable>();
    affectsTable->initAffects(this->storage->getStorageView());
    AffectsTTable *affectsTTable = this->storage->getTable<AffectsTTable>();
    affectsTTable->initAffectsT(this->storage->getStorageView());
    affectsTTable->populateAffectsT();
}
