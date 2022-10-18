#include "PKB.h"

#include "Algorithms/ControlFlowGraph.h"

PKB::PKB() {
    this->storage = new Storage();
    this->populateFacade = new PopulateFacade(this->storage);
    this->queryFacade = new QueryFacade(this->storage);
};

void PKB::populateNext() {
    NextTable *nextTable = this->storage->getTable<NextTable>();
    NextTTable *nextTTable = this->storage->getTable<NextTTable>();
    StorageView *storageView = this->storage->getStorageView();
    ControlFlowGraph *cfg =
        new ControlFlowGraph(nextTable, nextTTable, storageView);
    cfg->populateNext();
    this->cfg = cfg;
}

void PKB::populateNextT() { this->cfg->populateNextT(); }
