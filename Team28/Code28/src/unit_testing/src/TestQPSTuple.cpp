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
	REQUIRE(subTuples.size() == 2);
	Tuple first = subTuples[0];
	Tuple second = subTuples[1];
	REQUIRE(first.size() == 2);
	REQUIRE(first.values[0].value == "bar");
	REQUIRE(first.values[1].value == "2");
	REQUIRE(second.size() == 2);
}

TEST_CASE("Tuple can be combine sub-tuples correctly") {
	Value v1 = Value(ValueType::VAR_NAME, "foo");
	Value v2 = Value(ValueType::STMT_NUM, "5");
	Value v3 = Value(ValueType::VAR_NAME, "bar");
	Value v4 = Value(ValueType::STMT_NUM, "2");
	Tuple t1 = Tuple(std::vector<Value>{ v1, v2 });
	Tuple t2 = Tuple(std::vector<Value>{ v3, v4 });
	std::vector<Tuple> subTuples = std::vector<Tuple>{t1, t2};
	Tuple new_tuple = Tuple::combineSubTuples(subTuples);
	Tuple correct_tuple = Tuple(std::vector<Value>{ v1, v2, v3, v4 });
	REQUIRE(new_tuple.size() == 4);
	REQUIRE(new_tuple.equal(correct_tuple));
}