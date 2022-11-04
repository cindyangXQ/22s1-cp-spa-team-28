#pragma once

class Cache {
public:
    virtual void resetCache() = 0;

    void markForReset();
    void markTableResetted();

    bool shouldTableReset();

private:
    bool isTableUsed = false;
};
