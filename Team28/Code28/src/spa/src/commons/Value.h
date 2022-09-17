#pragma once
#include <iostream>

enum class ValueType { STMT_NUM, VAR_NAME, WILDCARD };

/*
 * Class encapsulating returned value in query.
 */
class Value {
public:
    ValueType type;
    std::string value;
    explicit Value();
    explicit Value(ValueType type, std::string value)
        : type(type), value(value){};

    bool operator==(const Value &other) const;
};
