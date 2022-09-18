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

namespace std {

  template <>
  struct hash<Value>
  {
    std::size_t operator()(const Value& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return ((hash<ValueType>()(k.type)
               ^ (hash<string>()(k.value) << 1)) >> 1);
    }
  };

}

struct value_pair_hash {
    inline std::size_t operator()(const std::pair<Value,Value> &v) const {
        return ((std::hash<Value>()(v.first) 
            ^ (std::hash<Value>()(v.second) << 1)) >> 1);
    }
};