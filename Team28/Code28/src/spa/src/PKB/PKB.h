#pragma once

#include "Facades/PopulateFacade.h"
#include "Facades/QueryFacade.h"
#include "Storage.h"

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

private:
    Storage *storage;
};
