#pragma once

/*
 * An interface of tables which acts as our database.
 */
class Table {
public:
    template <typename T> void store(T *item) = 0;

    virtual ~Table(){};
};
