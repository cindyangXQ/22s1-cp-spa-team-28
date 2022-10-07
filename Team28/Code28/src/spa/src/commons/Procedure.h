#pragma once

#include <string>

#include "Entity.h"

/*
 * Class encapsulating a SIMPLE procedure.
 */
class Procedure : public Entity {
public:
    explicit Procedure(std::string name);
};
