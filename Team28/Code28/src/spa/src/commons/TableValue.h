#pragma once

#include "unordered_set"

/*
 * An interface representing a possible Value in Table.
 */
class TableValue {};

enum class Designation {
    STMT,
    ASSIGN,
    VAR,
    CONST,
    PROC,
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    MOD_S,
    MOD_P,
    USE_S,
    USE_P,
    CALL,
    CALL_T,
    B_IN,
    B_OUT,
    IF_C,
    WHILE_C,
    PROC_NAME
};

typedef std::unordered_set<Designation> DESIGNATION_TYPE_SET;

const DESIGNATION_TYPE_SET namedEntitiesSet = {
    Designation::VAR, Designation::CONST, Designation::PROC};