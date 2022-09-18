#include "Value.h"

Value::Value() {}

bool Value::operator==(const Value &other) const {
    return (this->type == other.type) && (this->value == other.value);
}

bool Value::operator<(const Value &other) const {
    return this->type == other.type
        ? this->value < other.value
        : this->type < other.type;
}