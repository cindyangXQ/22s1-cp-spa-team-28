#include "catch.hpp"

#include "PKB/Algorithms/ControlFlowGraph.h"
#include "PKB/Storage/Storage.h"

TEST_CASE("CFG Traverses Correctly - 1") {
    Storage *storage = new Storage();
    NextTable* nextTable = storage->getTable<NextTable>();
    StorageView* storageView = storage->getStorageView();
    ControlFlowGraph::populateNext(nextTable, storageView);
}