#include "PKB.h"

#include "Algorithms/ControlFlowGraph.h"

PKB::PKB() {
    this->storage = new Storage();
    this->populateFacade = new PopulateFacade(this->storage);
    this->queryFacade = new QueryFacade(this->storage);
};

void PKB::populateNext() {
    NextTable* nextTable = this->storage->getTable<NextTable>();
    StorageView* storageView = this->storage->getStorageView();
    ControlFlowGraph cfg = ControlFlowGraph(nextTable, storageView);
    cfg.populateNext();
}