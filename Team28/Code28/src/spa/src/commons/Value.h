#pragma once
#include <iostream>

enum class ValueType { STMT_NUM, VAR_NAME, WILDCARD };

/*
 * Class encapsulating returned value in query.
 */
class Value {
public:
    explicit Value();
    explicit Value(ValueType type, std::string value)
        : type(type), value(value){};

    bool operator==(const Value &other) const;
    bool operator<(const Value &other) const;
    ValueType getValueType() const;
    std::string getValue() const;

private:
    ValueType type;
    std::string value;
};

namespace std {

template <> struct hash<Value> {
    std::size_t operator()(const Value &k) const {
        using std::hash;
        using std::size_t;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:
        return ((hash<ValueType>()(k.getValueType()) ^
                 (hash<string>()(k.getValue()) << 1)) >>
                1);
    }
};

}

struct value_pair_hash {
    inline std::size_t operator()(const std::pair<Value, Value> &v) const {
        return ((std::hash<Value>()(v.first) ^
                 (std::hash<Value>()(v.second) << 1)) >>
                1);
    }
};

struct value_pair_sort {
    bool operator()(const std::pair<Value, Value> &left,
                    const std::pair<Value, Value> &right) {
        return left.first == right.first ? left.second < right.second
                                         : left.first < right.first;
    }
};
