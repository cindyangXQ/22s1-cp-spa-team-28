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
    PROC_NAME,
    NEXT,
    NEXT_T,
    AFFECTS,
    AFFECTS_T
};

typedef std::unordered_set<Designation> DesignationTypeSet;

const DesignationTypeSet NAMED_ENTITIES_SET = {
    Designation::VAR, Designation::CONST, Designation::PROC};

const DesignationTypeSet COND_PATTERN_SET = {Designation::IF_C,
                                             Designation::WHILE_C};
