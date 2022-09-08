#pragma once

/*
 * Enumerates the different design entity.
 */
enum class Relationship
{
    FOLLOWS,
    FOLLOWS_STAR,
    PARENT,
    PARENT_STAR,
    USES,
    MODIFIES,
    CALLS,
    CALLS_STAR,
    EMPTY
};