#include "QPS/Evaluator/Tuple.h"

#include <vector>

#include "catch.hpp"

TEST_CASE("Tuple is initialised correctly") {
	Tuple t = Tuple(std::vector<Value>{ Value(ValueType::VAR_NAME, "foo"), Value(ValueType::STMT_NUM, "2") });
	REQUIRE(t.size() == 2);
}

TEST_CASE("Tuple can be split into sub-tuples correctly") {
	Tuple t = Tuple(std::vector<Value>{ Value(ValueType::VAR_NAME, "foo"), Value(ValueType::STMT_NUM, "2"), Value(ValueType::STMT_NUM, "5"), Value(ValueType::VAR_NAME, "bar") });
	std::vector<Tuple> subTuples = t.splitTuple(std::vector<int>{3, 1});
	REQUIRE(subTuple.size() == 2);
	Tuple first = subTuples[0];
	Tuple second = subTuples[1];
	REQUIRE(first.size() == 2);
	REQUIRE(first.values[0].value == "bar");
	REQUIRE(first.values[1].value == "2");
	REQUIRE(second.size() == 2);
}