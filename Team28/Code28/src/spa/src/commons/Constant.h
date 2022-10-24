#pragma once

#include <string>

#include "Entity.h"

/*
 * Class encapsulating a SIMPLE constant value.
 */
class Constant : public Entity {
public:
    explicit Constant(std::string_view name);
};
