#pragma once

#include <vector>

#include "../Storage/Storage.h"

/*
 * A Facade class for SP->PKB interactions.
 */
class PopulateFacade {
public:
    /*
     * Explicit constructor for a PopulateFacade.
     */
    explicit PopulateFacade(Storage *storage);

    template <typename T>
    void store(std::vector<T *> *items, Populate popType) {
        Table *table = this->storage->getStoreTable(popType);
        for (T *item : *items) {
            table->store(item);
        }
    };

private:
    Storage *storage;

    template <typename T, typename Subclass>
    void storeHelper(std::vector<T *> *items) {
        Table *table = this->storage->getTable<Subclass>();

        for (T *item : *items) {
            table->store(item);
        }
    }
};
