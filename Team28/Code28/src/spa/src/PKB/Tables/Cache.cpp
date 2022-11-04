#include "Cache.h"

void Cache::markForReset() { isTableUsed = true; }
void Cache::markTableResetted() { isTableUsed = false; }

bool Cache::shouldTableReset() { return isTableUsed; }
