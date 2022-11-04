#pragma once

#include "Algorithms/ControlFlowGraph.h"
#include "Facades/PopulateFacade.h"
#include "Facades/QueryFacade.h"
#include "Storage/Storage.h"

/*
 * A wrapper class for PKB.
 * External components will utilise this class to retrieve the respective
 * facades for the PKB APIs.
 */
class PKB {
public:
    PopulateFacade *populateFacade;
    QueryFacade *queryFacade;

    /*
     * Explicit constructor for PKB.
     */
    explicit PKB();

    /*
     * Populates the NextTable based on existing relationships in knowledge
     * base.
     */
    void populateNext();

    void initNextT();

    void initAffects();

private:
    ControlFlowGraph *cfg;
    Storage *storage;
};
